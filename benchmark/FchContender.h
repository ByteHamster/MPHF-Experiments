#pragma once

#include <hasher/hasher.hpp>
#include <fch.hpp>
#include "Contender.h"

class FchContender : public Contender {
    public:
        double c;
        mphf::FCH<mphf::hasher::Hasher<>> fchFunction;
        mphf::FCH<mphf::hasher::Hasher<>>::Builder builder;

        FchContender(size_t N, double loadFactor, double c)
                : Contender(N, loadFactor), c(c), builder(c) {
        }

        std::string name() override {
            return std::string("Fch")
                    + " c=" + std::to_string(c);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            builder.build(fchFunction, keys);
        }

        size_t sizeBits() override {
            return fchFunction.num_bits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, fchFunction);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, fchFunction);
        }
};

void fchPtHashContenderRunner(size_t N, double loadFactor) {
    for (double c = 3.0; c < 12.0; c += 0.4) {
        FchContender(N, loadFactor, c).run();
    }
}
