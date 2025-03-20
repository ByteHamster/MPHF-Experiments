#pragma once

#include "Contender.h"
#include <MorphisHash.h>

template<int l, int ws>
class MorphisHashContender : public Contender {
    public:
        size_t bucketSize;
        morphishash::MorphisHash<l, ws> *morphisHash = nullptr;

        MorphisHashContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~MorphisHashContender() override {
            delete morphisHash;
        }

        std::string name() override {
            return std::string("MorphisHash")
                  + " l=" + std::to_string(l)
                  + " ws=" + std::to_string(ws)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            morphisHash = new morphishash::MorphisHash<l, ws>(keys, bucketSize, numThreads);
        }

        size_t sizeBits() override {
            return morphisHash->getBits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, *morphisHash);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, *morphisHash);
        }
};

void morphisHashContenderRunner(size_t N);
