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

void shockHashSimdContenderRunner(size_t N);
