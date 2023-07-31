#pragma once

#include <memory> // Otherwise PTHash misses an import
#include <pthash.hpp>
#undef MAX_BUCKET_SIZE
#include "Contender.h"

template <bool minimal, typename encoder>
class PTHashContender : public Contender {
    public:
        double c;
        double internalLoadFactor;
        pthash::single_phf<pthash::murmurhash2_64, encoder, minimal> pthashFunction;

        PTHashContender(size_t N, double loadFactor, double c)
                : Contender(N, minimal ? 1.0 : loadFactor), c(c), internalLoadFactor(loadFactor) {
        }

        std::string name() override {
            return std::string("PTHash")
                    + " minimal=" + std::to_string(minimal)
                    + " c=" + std::to_string(c)
                    + " encoder=" + encoder::name()
                    + " lf=" + std::to_string(internalLoadFactor);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            pthash::build_configuration config;
            config.c = c;
            config.alpha = internalLoadFactor;
            config.num_threads = numThreads;
            config.minimal_output = minimal;
            config.verbose_output = false;
            pthashFunction.build_in_internal_memory(keys.begin(), keys.size(), config);
        }

        size_t sizeBits() override {
            return pthashFunction.num_bits();
        }

        void performQueries(const std::vector<std::string> &keys) override {
            doPerformQueries(keys, pthashFunction);
        }

        void performTest(const std::vector<std::string> &keys) override {
            doPerformTest(keys, pthashFunction);
        }
};

void ptHashContenderRunner(size_t N, double loadFactor, bool minimalOnly = false) {
    for (double c = 3.0; c < 12.0; c += 0.4) {
        if (!minimalOnly) {
            PTHashContender<false, pthash::elias_fano>(N, loadFactor, c).run();
            PTHashContender<false, pthash::dictionary_dictionary>(N, loadFactor, c).run();
            PTHashContender<false, pthash::dictionary_elias_fano>(N, loadFactor, c).run();
        }

        PTHashContender<true, pthash::elias_fano>(N, loadFactor, c).run();
        PTHashContender<true, pthash::dictionary_dictionary>(N, loadFactor, c).run();
        PTHashContender<true, pthash::dictionary_elias_fano>(N, loadFactor, c).run();
    }
}
