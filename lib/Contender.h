#pragma once

#include <random>
#include <iostream>
#include <chrono>
#include <bytehamster/util/XorShift64.h>
#include <unistd.h>
#include <thread>
#include <span>

#define DO_NOT_OPTIMIZE(value) asm volatile ("" : : "r,m"(value) : "memory")

class Contender {
    public:
        static size_t numQueries;
        static size_t numThreads;
        static size_t numQueryThreads;
        static size_t seed;
        static bool skipTests;

        const size_t N;
        const double loadFactor;
        const double mByN;
        const size_t M;
        long constructionTimeMicroseconds = 0;
        long queryTimeMilliseconds = 0;

        Contender(size_t N, double loadFactor)
                : N(N), loadFactor(loadFactor), mByN(1.0 / loadFactor), M(N * mByN) {
        }

        virtual ~Contender() = default;

        virtual std::string name() = 0;
        virtual size_t sizeBits() = 0;
        virtual void construct(const std::vector<std::string> &keys) = 0;

        virtual void beforeConstruction(const std::vector<std::string> &keys) {
            (void) keys;
        }

        virtual void beforeQueries(const std::span<std::string> &keys) {
            (void) keys;
        }

        virtual void performQueries(const std::span<std::string> keys) = 0;
        virtual void performTest(const std::span<std::string> keys) = 0;

        void run(bool shouldPrintResult = true); // In cpp file

        void printResult(std::string additional = ""); // In cpp file

    protected:
        template<typename F>
        void doPerformQueries(const std::span<std::string> keys, F &hashFunction) {
            for (const std::string &key : keys) {
                size_t retrieved = hashFunction(const_cast<std::string &>(key));
                // Some contenders expect non-const keys but actually use them as const.
                DO_NOT_OPTIMIZE(retrieved);
            }
        }

        template<typename F>
        void doPerformTest(const std::span<std::string> keys, F &hashFunction) {
            double eps = 1.0001; // Rounding with load factor variables
            std::vector<bool> taken(M * eps);
            for (size_t i = 0; i < keys.size(); i++) {
                // Some contenders expect non-const keys but actually use them as const.
                size_t retrieved = hashFunction(const_cast<std::string &>(keys[i]));
                if (retrieved > M * eps) {
                    std::cout << "Error: Range wrong. Hash function returned " << retrieved
                            << " but maximum should be " << (M*eps) << " (actually " << M << ")" << std::endl;
                    throw std::logic_error("Range wrong");
                }
                if (taken[retrieved]) {
                    std::cout<<"Error: Collision: Key #"<<i<<"/"<<N<<" resulted in "<<retrieved<<std::endl;
                    std::cout<<"Aborting query"<<std::endl;
                    throw std::logic_error("Collision");
                }
                taken[retrieved] = true;
            }
        }
};
