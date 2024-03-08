#pragma once

#include <dense_partitioned_phf.hpp>
#include <encoders/encoders.hpp>
#undef MAX_BUCKET_SIZE
#include "Contender.h"

template <bool minimal, typename encoder, typename bucketer = pthash::skew_bucketer>
class DensePartitionedPTHashContender : public Contender {
    public:
        double lambda;
        double internalLoadFactor;
        size_t partitionSize = 1500;
        pthash::dense_partitioned_phf<pthash::murmurhash2_64, bucketer, encoder, minimal> pthashFunction;

        DensePartitionedPTHashContender(size_t N, double loadFactor, double lambda)
                : Contender(N, minimal ? 1.0 : loadFactor), lambda(lambda), internalLoadFactor(loadFactor) {
        }

        std::string name() override {
            return std::string("DensePartitionedPTHash")
                    + " minimal=" + std::to_string(minimal)
                    + " lambda=" + std::to_string(lambda)
                    + " encoder=" + encoder::name()
                    + " partitionSize=" + std::to_string(partitionSize)
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
            config.num_partitions = N / partitionSize;
            config.minimal_output = minimal;
            config.verbose_output = false;
            config.dense_partitioning = true;
            config.secondary_sort = true;
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

