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

template <int l>
void bipartiteShockHashTestMulti(size_t N) {
    {BipartiteShockHashContender<l>(N, 100).run();}
    {BipartiteShockHashContender<l>(N, 500).run();}
    {BipartiteShockHashContender<l>(N, 2000).run();}
}

void bipartiteShockHashContenderRunner(size_t N) {
    bipartiteShockHashTestMulti<28>(N);
    bipartiteShockHashTestMulti<34>(N);
    bipartiteShockHashTestMulti<40>(N);
    bipartiteShockHashTestMulti<46>(N);
    bipartiteShockHashTestMulti<52>(N);
    bipartiteShockHashTestMulti<58>(N);
    bipartiteShockHashTestMulti<64>(N);
    bipartiteShockHashTestMulti<70>(N);
    bipartiteShockHashTestMulti<76>(N);
    bipartiteShockHashTestMulti<82>(N);
    bipartiteShockHashTestMulti<88>(N);
    bipartiteShockHashTestMulti<94>(N);
}
