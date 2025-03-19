#pragma once

#include "Contender.h"

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#include <hasher/hasher.hpp>
#include <fch.hpp>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

class FchContender : public Contender {
    public:
        double c;
        mphf::FCH<mphf::hasher::Hasher<>> fchFunction;
        mphf::FCH<mphf::hasher::Hasher<>>::Builder builder;

        FchContender(size_t N, double c)
                : Contender(N, 1.0), c(c), builder(c) {
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

void fchPtHashContenderRunner(size_t N);
