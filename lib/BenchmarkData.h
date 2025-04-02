#pragma once
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
