#pragma once

#include "Contender.h"
#include <Fips.h>

class FiPSContender : public Contender {
    public:
        fips::FiPS<> *fips = nullptr;
        double gamma;

        FiPSContender(size_t N, double gamma)
                : Contender(N, 1.0), gamma(gamma) {
        }

        ~FiPSContender() override {
            delete fips;
        }

        std::string name() override {
            return std::string("FiPS")
                + " gamma=" + std::to_string(gamma);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            fips = new fips::FiPS<>(keys, gamma);
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

void fiPSContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 2.5; gamma += 0.15) {
        FiPSContender(N, gamma).run();
    }
}