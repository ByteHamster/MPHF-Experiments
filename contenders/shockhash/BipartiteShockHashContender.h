#pragma once

#include "Contender.h"
#include <ShockHash2.h>

template<int l>
class BipartiteShockHashContender : public Contender {
    public:
        size_t bucketSize;
        shockhash::ShockHash2<l> *shockHash = nullptr;

        BipartiteShockHashContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~BipartiteShockHashContender() override {
            delete shockHash;
        }

        std::string name() override {
            return std::string("BipartiteShockHash")
                  + " l=" + std::to_string(l)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            shockHash = new shockhash::ShockHash2<l>(keys, bucketSize, numThreads);
        }

        size_t sizeBits() override {
            return shockHash->getBits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, *shockHash);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, *shockHash);
        }
};

void bipartiteShockHashContenderRunner(size_t N);
