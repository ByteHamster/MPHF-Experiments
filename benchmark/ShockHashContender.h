#pragma once

#include "Contender.h"
#include <ShockHash.h>

template<int l>
class ShockHashContender : public Contender {
    public:
        size_t bucketSize;
        shockhash::ShockHash<l> *recSplit = nullptr;

        ShockHashContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~ShockHashContender() override {
            delete recSplit;
        }

        std::string name() override {
            return std::string("ShockHash")
                  + " l=" + std::to_string(l)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            recSplit = new shockhash::ShockHash<l>(keys, bucketSize);
        }

        size_t sizeBits() override {
            return recSplit->getBits();
        }

        void performQueries(const std::vector<std::string> &keys) override {
            doPerformQueries(keys, *recSplit);
        }

        void performTest(const std::vector<std::string> &keys) override {
            doPerformTest(keys, *recSplit);
        }
};

template <int l>
void shockHashTestMulti(size_t N) {
    {ShockHashContender<l>(N, 100).run();}
    {ShockHashContender<l>(N, 500).run();}
    {ShockHashContender<l>(N, 2000).run();}
}

void shockHashContenderRunner(size_t N) {
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
}
