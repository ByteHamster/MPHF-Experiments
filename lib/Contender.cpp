#include "Contender.h"

#include <vector>
#include <iostream>
#include <chrono>
#include <bytehamster/util/XorShift64.h>

std::vector<std::string> generateInputData(size_t N, uint64_t seed) {
    std::vector<std::string> inputData;
    inputData.reserve(N);
    bytehamster::util::XorShift64 prng(seed);
    std::cout<<"Generating input"<<std::flush;
    char string[200];
    for (size_t i = 0; i < N; i++) {
        if ((i % (N/5)) == 0) {
            std::cout<<"\rGenerating input: "<<100l*i/N<<"%"<<std::flush;
        }
        size_t length = 10 + prng((30 - 10) * 2);
        for (std::size_t k = 0; k < (length + sizeof(uint64_t))/sizeof(uint64_t); ++k) {
            ((uint64_t*) string)[k] = prng();
        }
        // Repair null bytes
        for (std::size_t k = 0; k < length; ++k) {
            if (string[k] == 0) {
                string[k] = 1 + prng(254);
            }
        }
        string[length] = 0;
        inputData.emplace_back(string, length);
    }
    std::cout<<"\rInput generation complete."<<std::endl;
    return inputData;
}

void Contender::run(bool shouldPrintResult) {
    std::cout <<  std::endl;
    std::cout << "Contender: " << name().substr(0, name().find(' ')) << std::endl;

    if (seed == 0) {
        auto time = std::chrono::system_clock::now();
        seed = std::chrono::duration_cast<std::chrono::milliseconds>(time.time_since_epoch()).count();
    }
    bytehamster::util::XorShift64 prng(seed);
    for (size_t i = 0; i < 10; i++) {
        prng(); // Ensure that first few generated seeds don't have too many zeroes when users pick small seeds
    }
    std::cout << "Seed: " << seed << std::endl;
    std::vector<std::string> keys = generateInputData(N, prng());
    beforeConstruction(keys);

    std::cout << "Cooldown" << std::endl;
    usleep(1000*1000);
    std::cout << "Constructing" << std::endl;

    std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
    try {
        construct(keys);
    } catch (const std::exception& e) {
        std::cout<<"Error: "<<e.what()<<std::endl;
        return;
    }
    std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
    constructionTimeMicroseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();

    if (!skipTests) {
        std::cout<<"Testing"<<std::endl;
        performTest(keys);
    }

    queryTimeMilliseconds = 0;
    if (numQueries > 0) {
        std::cout<<"Preparing query plan"<<std::endl;
        std::vector<std::string> queryPlan;
        queryPlan.reserve(numQueries * numQueryThreads);
        for (size_t i = 0; i < numQueries * numQueryThreads; i++) {
            queryPlan.push_back(keys[prng(N)]);
        }
        beforeQueries(queryPlan);
        std::cout << "Cooldown" << std::endl;
        usleep(1000*1000);
        std::cout<<"Querying"<<std::endl;
        if (numQueryThreads == 1) {
            begin = std::chrono::steady_clock::now();
            performQueries(queryPlan);
            end = std::chrono::steady_clock::now();
            queryTimeMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        } else {
            std::vector<std::thread> threads;
            begin = std::chrono::steady_clock::now();
            for (size_t i = 0; i < numQueryThreads; i++) {
                std::span<std::string> querySpan(queryPlan.begin() + i * numQueries,
                                                 queryPlan.begin() + (i + 1) * numQueries);
                threads.emplace_back([&querySpan, this] {
                    performQueries(querySpan);
                });
            }
            for (size_t i = 0; i < numQueryThreads; i++) {
                threads.at(i).join();
            }
            end = std::chrono::steady_clock::now();
            queryTimeMilliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
        }
    }
    if (shouldPrintResult) {
        printResult();
    }
}

void Contender::printResult(std::string additional) {
    // Some competitors print stuff when determining their space consumption
    double bitsPerElement = (double) sizeBits() / N;
    std::cout << "RESULT"
              << " name=" << name()
              << " bitsPerElement=" << bitsPerElement
              << " constructionTimeMilliseconds=" << (constructionTimeMicroseconds < 10000
                                    ? std::to_string(0.001 * constructionTimeMicroseconds)
                                    : std::to_string(constructionTimeMicroseconds / 1000))
              << " queryTimeMilliseconds=" << queryTimeMilliseconds
              << " numQueries=" << numQueries
              << " numQueriesTotal=" << (numQueries * numQueryThreads)
              << " N=" << N
              << " loadFactor=" << loadFactor
              << " threads=" << numThreads
              << " queryThreads=" << numQueryThreads
              << additional
              << std::endl;
}

size_t Contender::numQueries = 5e7;
size_t Contender::numThreads = 1;
size_t Contender::numQueryThreads = 1;
size_t Contender::seed = 0;
bool Contender::skipTests = false;
