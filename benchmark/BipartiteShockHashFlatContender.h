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
    {BipartiteShockHashFlatContender<46>(N).run();}
    {BipartiteShockHashFlatContender<50>(N).run();}
    {BipartiteShockHashFlatContender<54>(N).run();}
    {BipartiteShockHashFlatContender<58>(N).run();}
    {BipartiteShockHashFlatContender<62>(N).run();}
    {BipartiteShockHashFlatContender<66>(N).run();}
    {BipartiteShockHashFlatContender<70>(N).run();}
    {BipartiteShockHashFlatContender<74>(N).run();}
    {BipartiteShockHashFlatContender<78>(N).run();}
    {BipartiteShockHashFlatContender<82>(N).run();}
    {BipartiteShockHashFlatContender<84>(N).run();}
}