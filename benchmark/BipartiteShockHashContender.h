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

        void performQueries(const std::vector<std::string> &keys) override {
            doPerformQueries(keys, *shockHash);
        }

        void performTest(const std::vector<std::string> &keys) override {
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
    bipartiteShockHashTestMulti<16>(N);
    bipartiteShockHashTestMulti<24>(N);
    bipartiteShockHashTestMulti<32>(N);
    bipartiteShockHashTestMulti<40>(N);
    bipartiteShockHashTestMulti<48>(N);
    bipartiteShockHashTestMulti<56>(N);
    bipartiteShockHashTestMulti<64>(N);
    bipartiteShockHashTestMulti<72>(N);
    bipartiteShockHashTestMulti<80>(N);
    bipartiteShockHashTestMulti<88>(N);
    bipartiteShockHashTestMulti<96>(N);
    bipartiteShockHashTestMulti<104>(N);
    bipartiteShockHashTestMulti<112>(N);
    bipartiteShockHashTestMulti<120>(N);
    bipartiteShockHashTestMulti<128>(N);
}
