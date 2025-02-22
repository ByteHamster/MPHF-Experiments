#pragma once

#include "Contender.h"
#include <ShockHash2Flat.h>

template<int l>
class BipartiteShockHashFlatContender : public Contender {
    public:
        shockhash::ShockHash2Flat<l> *shockHash = nullptr;

        BipartiteShockHashFlatContender(size_t N)
                : Contender(N, 1.0) {
        }

        ~BipartiteShockHashFlatContender() override {
            delete shockHash;
        }

        std::string name() override {
            return std::string("BipartiteShockHashFlat")
                  + " l=" + std::to_string(l);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            shockHash = new shockhash::ShockHash2Flat<l>(keys);
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

void bipartiteShockHashFlatContenderRunner(size_t N) {
    {BipartiteShockHashFlatContender<28>(N).run();}
    {BipartiteShockHashFlatContender<34>(N).run();}
    {BipartiteShockHashFlatContender<40>(N).run();}
    {BipartiteShockHashFlatContender<46>(N).run();}
    {BipartiteShockHashFlatContender<52>(N).run();}
    {BipartiteShockHashFlatContender<58>(N).run();}
    {BipartiteShockHashFlatContender<64>(N).run();}
    {BipartiteShockHashFlatContender<70>(N).run();}
    {BipartiteShockHashFlatContender<76>(N).run();}
    {BipartiteShockHashFlatContender<82>(N).run();}
    {BipartiteShockHashFlatContender<88>(N).run();}
    {BipartiteShockHashFlatContender<94>(N).run();}
}