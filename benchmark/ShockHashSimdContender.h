#pragma once

#include "Contender.h"
#include <SIMDShockHash.hpp>

template<int l, bool rotationFitting = true>
class ShockHashSimdContender : public Contender {
    public:
        size_t bucketSize;
        shockhash::SIMDShockHash<l, rotationFitting> *shockHash = nullptr;

        ShockHashSimdContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~ShockHashSimdContender() override {
            delete shockHash;
        }

        std::string name() override {
            return std::string("ShockHashSIMD")
                  + " rotationFitting=" + std::to_string(rotationFitting)
                  + " l=" + std::to_string(l)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            shockHash = new shockhash::SIMDShockHash<l, rotationFitting>(keys, bucketSize);
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
void shockHashSimdTestMulti(size_t N) {
    {ShockHashSimdContender<l>(N, 100).run();}
    {ShockHashSimdContender<l>(N, 500).run();}
    {ShockHashSimdContender<l>(N, 2000).run();}
}

void shockHashSimdContenderRunner(size_t N) {
    shockHashSimdTestMulti<4>(N);
    shockHashSimdTestMulti<6>(N);
    shockHashSimdTestMulti<8>(N);
    shockHashSimdTestMulti<10>(N);
    shockHashSimdTestMulti<12>(N);
    shockHashSimdTestMulti<15>(N);
    shockHashSimdTestMulti<24>(N);
    shockHashSimdTestMulti<28>(N);
    shockHashSimdTestMulti<32>(N);
    shockHashSimdTestMulti<34>(N);
    shockHashSimdTestMulti<36>(N);
    shockHashSimdTestMulti<40>(N);
    shockHashSimdTestMulti<42>(N);
    shockHashSimdTestMulti<44>(N);
    shockHashSimdTestMulti<46>(N);
    shockHashSimdTestMulti<48>(N);
    shockHashSimdTestMulti<50>(N);
    shockHashSimdTestMulti<52>(N);
    shockHashSimdTestMulti<54>(N);
}
