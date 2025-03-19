#pragma once

#ifdef __GNUC__
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wmaybe-uninitialized"
#endif
#include <BooPHF.h>
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif

#include <bytehamster/util/MurmurHash64.h>
#include "Contender.h"

class BBHashContender : public Contender {
    public:
        typedef boomphf::SingleHashFunctor<u_int64_t> hasher_t;
        typedef boomphf::mphf<u_int64_t, hasher_t> boophf_t;
        boophf_t * bbhash = nullptr;
        std::vector<u_int64_t> mhcs;
        double gamma;
        double perc_elem_loaded;

        BBHashContender(size_t N, double gamma, double perc_elem_loaded = 0.03)
                : Contender(N, 1.0), gamma(gamma), perc_elem_loaded(perc_elem_loaded) {
        }

        ~BBHashContender() override {
            delete bbhash;
        }

        std::string name() override {
            return std::string("BBHash")
                + " gamma=" + std::to_string(gamma)
                + " perc_elem_loaded=" + std::to_string(perc_elem_loaded);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            for (const std::string &s : keys) {
                mhcs.emplace_back(bytehamster::util::MurmurHash64(s));
            }
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            bbhash = new boomphf::mphf<u_int64_t,hasher_t>(mhcs.size(), mhcs,
                    /* num_thread */ numThreads, /* gamma */ gamma, /* writeEach */ true,
                    /* progress */ false, perc_elem_loaded);
        }

        size_t sizeBits() override {
            return bbhash->totalBitSize();
        }

        void performQueries(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return bbhash->lookup(bytehamster::util::MurmurHash64(key));
            };
            doPerformQueries(keys, x);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return bbhash->lookup(bytehamster::util::MurmurHash64(key));
            };
            doPerformTest(keys, x);
        }
};

void bbHashContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 5.0; gamma += 0.15) {
        for (double perc_elem_loaded = 0.05; perc_elem_loaded < 1.0; perc_elem_loaded += 0.8) {
            BBHashContender(N, gamma, perc_elem_loaded).run();
        }
        BBHashContender(N, gamma, 0).run();
    }
}