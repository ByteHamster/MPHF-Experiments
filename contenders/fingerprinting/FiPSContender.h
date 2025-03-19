#pragma once

#include "Contender.h"
#include <Fips.h>

template <size_t _lineSize = 256, typename _offsetType = uint16_t, bool _useUpperRank = true>
class FiPSContender : public Contender {
    public:
        fips::FiPS<_lineSize, _offsetType, _useUpperRank> *fips = nullptr;
        double gamma;

        FiPSContender(size_t N, double gamma)
                : Contender(N, 1.0), gamma(gamma) {
        }

        ~FiPSContender() override {
            delete fips;
        }

        std::string name() override {
            return std::string("FiPS")
                + " gamma=" + std::to_string(gamma)
                + " lineSize=" + std::to_string(_lineSize)
                + " offsetSize=" + std::to_string(8 * sizeof(_offsetType))
                + " upperRank=" + std::to_string(_useUpperRank);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            fips = new fips::FiPS<_lineSize, _offsetType, _useUpperRank>(keys, gamma);
        }

        size_t sizeBits() override {
            return fips->getBits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, *fips);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, *fips);
        }
};

void fiPSContenderRunner(size_t N);
