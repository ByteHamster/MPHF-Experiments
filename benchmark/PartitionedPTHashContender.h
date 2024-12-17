#pragma once

#include <partitioned_phf.hpp>
#undef MAX_BUCKET_SIZE
#include "Contender.h"

template <bool minimal, typename encoder, typename bucketer = pthash::skew_bucketer>
class PartitionedPTHashContender : public Contender {
    public:
        double lambda;
        double internalLoadFactor;
        pthash::partitioned_phf<pthash::murmurhash2_64, bucketer, encoder, minimal, pthash::xor_displacement> pthashFunction;

        PartitionedPTHashContender(size_t N, double loadFactor, double lambda)
                : Contender(N, minimal ? 1.0 : loadFactor), lambda(lambda), internalLoadFactor(loadFactor) {
        }

        std::string name() override {
            return std::string("PartitionedPTHash")
                    + " minimal=" + std::to_string(minimal)
                    + " lambda=" + std::to_string(lambda)
                    + " encoder=" + encoder::name()
                    + " lf=" + std::to_string(internalLoadFactor);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            pthash::build_configuration config;
            config.lambda = lambda;
            config.alpha = internalLoadFactor;
            config.num_threads = numThreads;
            config.avg_partition_size = (N + numThreads) / numThreads;
            config.minimal = minimal;
            config.verbose = false;
            config.search = pthash::xor_displacement;
            pthashFunction.build_in_internal_memory(keys.begin(), keys.size(), config);
        }

        size_t sizeBits() override {
            return pthashFunction.num_bits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, pthashFunction);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, pthashFunction);
        }
};

void partitionedPtHashContenderRunner(size_t N, double loadFactor, bool minimalOnly) {
    for (double lambda = 3.0; lambda < 10.0; lambda += 0.4) {
        if (!minimalOnly) {
            PartitionedPTHashContender<false, pthash::elias_fano>(N, loadFactor, lambda).run();
            PartitionedPTHashContender<false, pthash::dictionary_dictionary>(N, loadFactor, lambda).run();
            PartitionedPTHashContender<false, pthash::dictionary_elias_fano>(N, loadFactor, lambda).run();
        }
        PartitionedPTHashContender<true, pthash::elias_fano>(N, loadFactor, lambda).run();
        PartitionedPTHashContender<true, pthash::dictionary_dictionary>(N, loadFactor, lambda).run();
        PartitionedPTHashContender<true, pthash::dictionary_elias_fano>(N, loadFactor, lambda).run();
    }
}
