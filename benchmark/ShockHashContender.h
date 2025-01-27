#pragma once

#include "Contender.h"
#include <ShockHash.h>

template<int l, bool rotationFitting = true>
class ShockHashContender : public Contender {
    public:
        size_t bucketSize;
        shockhash::ShockHash<l, rotationFitting> *shockHash = nullptr;

        ShockHashContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~ShockHashContender() override {
            delete shockHash;
        }

        std::string name() override {
            return std::string("ShockHash")
                  + " rotationFitting=" + std::to_string(rotationFitting)
                  + " l=" + std::to_string(l)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            shockHash = new shockhash::ShockHash<l, rotationFitting>(keys, bucketSize);
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
void shockHashTestMulti(size_t N) {
    {ShockHashContender<l>(N, 100).run();}
    {ShockHashContender<l>(N, 500).run();}
    {ShockHashContender<l>(N, 2000).run();}
}

void shockHashContenderRunner(size_t N) {
    shockHashTestMulti<4>(N);
    shockHashTestMulti<6>(N);
    shockHashTestMulti<8>(N);
    shockHashTestMulti<10>(N);
    shockHashTestMulti<12>(N);
    shockHashTestMulti<15>(N);
    shockHashTestMulti<24>(N);
    shockHashTestMulti<28>(N);
    shockHashTestMulti<32>(N);
    shockHashTestMulti<34>(N);
    shockHashTestMulti<36>(N);
    shockHashTestMulti<40>(N);
    shockHashTestMulti<42>(N);
    shockHashTestMulti<44>(N);
    shockHashTestMulti<46>(N);
    shockHashTestMulti<48>(N);
    shockHashTestMulti<50>(N);
    shockHashTestMulti<52>(N);
    shockHashTestMulti<54>(N);
    shockHashTestMulti<58>(N);
}
