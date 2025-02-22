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

template <int l, int ws>
void morphisHashTestMultiBucket(size_t N) {
    {MorphisHashContender<l, ws>(N, 100).run();}
    {MorphisHashContender<l, ws>(N, 500).run();}
    {MorphisHashContender<l, ws>(N, 2000).run();}
}

template <int l>
void morphisHashTestMultiWidth(size_t N) {
    morphisHashTestMultiBucket<l, 1>(N);
    morphisHashTestMultiBucket<l, 2>(N);
    morphisHashTestMultiBucket<l, 3>(N);
    morphisHashTestMultiBucket<l, 4>(N);
    morphisHashTestMultiBucket<l, 5>(N);
    morphisHashTestMultiBucket<l, 6>(N);
}

void morphisHashContenderRunner(size_t N) {
    morphisHashTestMultiWidth<28>(N);
    morphisHashTestMultiWidth<34>(N);
    morphisHashTestMultiWidth<40>(N);
    morphisHashTestMultiWidth<46>(N);
    morphisHashTestMultiWidth<52>(N);
    morphisHashTestMultiWidth<58>(N);
    morphisHashTestMultiWidth<64>(N);
    morphisHashTestMultiWidth<70>(N);
    morphisHashTestMultiWidth<76>(N);
    morphisHashTestMultiWidth<82>(N);
}
