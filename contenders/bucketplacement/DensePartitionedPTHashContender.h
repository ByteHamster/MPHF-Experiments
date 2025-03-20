#pragma once

#include <memory> // Otherwise PTHash misses an import
#include <pthash.hpp>
#include <dense_partitioned_phf.hpp>
#include <utils/encoders.hpp>
#undef MAX_BUCKET_SIZE
#include "Contender.h"

template <typename encoder, typename bucketer = pthash::skew_bucketer>
class DensePartitionedPTHashContender : public Contender {
    public:
        double lambda;
        double internalLoadFactor;
        size_t partitionSize = 2500;
        pthash::dense_partitioned_phf<pthash::xxhash128, bucketer, encoder, false, pthash::add_displacement> pthashFunction;
        pthash::build_timings buildTimings;

        DensePartitionedPTHashContender(size_t N, double loadFactor, double lambda)
                : Contender(N, 1.0), lambda(lambda), internalLoadFactor(loadFactor) {
        }

        std::string name() override {
            return std::string("DensePartitionedPTHash")
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
            config.avg_partition_size = partitionSize;
            config.minimal = true;
            config.verbose = false;
            config.dense_partitioning = true;
            config.secondary_sort = true;
            config.search = pthash::add_displacement;
            buildTimings = pthashFunction.build_in_internal_memory(keys.begin(), keys.size(), config);
        }

        size_t sizeBits() override {
            std::cout << "partitioning: " << buildTimings.partitioning_microseconds / 1000000 << " [sec]" << std::endl;
            std::cout << "mapping+ordering: " << buildTimings.mapping_ordering_microseconds / 1000000 << " [sec]" << std::endl;
            std::cout << "searching: " << buildTimings.searching_microseconds / 1000000 << " [sec]" << std::endl;
            std::cout << "encoding: " << buildTimings.encoding_microseconds / 1000000 << " [sec]" << std::endl;
            return pthashFunction.num_bits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, pthashFunction);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, pthashFunction);
        }
};

void densePartitionedPtHashContenderRunner(size_t N);
