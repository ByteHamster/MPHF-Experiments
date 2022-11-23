#pragma once

#include <PartitionedSicHash.h>
#include "Contender.h"

template<bool minimal, size_t ribbonWidth, int minimalFanoLowerBits = 3>
class PartitionedSicHashContender : public Contender {
    public:
        sichash::PartitionedSicHash<minimal, ribbonWidth, minimalFanoLowerBits> *perfectHashing = nullptr;
        sichash::SicHashConfig config;

        PartitionedSicHashContender(size_t N, double loadFactor, sichash::SicHashConfig config)
                : Contender(N, minimal ? 1.0 : loadFactor), config(config) {
            this->config.loadFactor = loadFactor;
        }

        ~PartitionedSicHashContender() override {
            delete perfectHashing;
        }

        std::string name() override {
            return std::string("PartitionedSicHash")
                   + " minimal=" + std::to_string(minimal)
                   + " lf=" + std::to_string(config.loadFactor) // Internal load factor
                   + " t1=" + std::to_string(config.class1Percentage())
                   + " t2=" + std::to_string(config.class2Percentage())
                   + (config.x >= 0 ? " spaceBudgetX=" + std::to_string(config.x) : "")
                   + " ribbonWidth=" + std::to_string(ribbonWidth)
                   + " minimalFanoLowerBits=" + std::to_string(minimalFanoLowerBits)
                   + " bucketSize=" + std::to_string(config.smallTableSize);
        }

        void construct(const std::vector<std::string> &keys) override {
            perfectHashing = new sichash::PartitionedSicHash<minimal, ribbonWidth, minimalFanoLowerBits>(keys, config, numThreads);
        }

        size_t sizeBits() override {
            return perfectHashing->spaceUsage();
        }

        void performQueries(const std::vector<std::string> &keys) override {
            doPerformQueries(keys, *perfectHashing);
        }

        void performTest(const std::vector<std::string> &keys) override {
            doPerformTest(keys, *perfectHashing);
        }
};

template <size_t ribbonWidth>
void partitionedSicHashContenderRunner(size_t N, double loadFactor, bool minimalOnly = false) {
    for (float spaceBudget = 1.35; spaceBudget < 3.0; spaceBudget += 0.03) {
        for (float x = 0.0; x < 1.0; x += 0.2) {
            if (!minimalOnly) {
                {PartitionedSicHashContender<false, ribbonWidth>(N, loadFactor, sichash::SicHashConfig().spaceBudget(spaceBudget, x)).run();}
            }

            if (loadFactor < 0.89) {
                {PartitionedSicHashContender<true, ribbonWidth, 3>(N, loadFactor, sichash::SicHashConfig().spaceBudget(spaceBudget, x)).run();}
            } else if (loadFactor < 0.94) {
                {PartitionedSicHashContender<true, ribbonWidth, 4>(N, loadFactor, sichash::SicHashConfig().spaceBudget(spaceBudget, x)).run();}
            } else {
                {PartitionedSicHashContender<true, ribbonWidth, 5>(N, loadFactor, sichash::SicHashConfig().spaceBudget(spaceBudget, x)).run();}
            }
        }
    }
}
