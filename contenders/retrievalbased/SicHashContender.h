#pragma once

#include <PartitionedSicHash.h>
#include "Contender.h"

template<bool minimal, size_t ribbonWidth, int minimalFanoLowerBits = 3>
class SicHashContender : public Contender {
    public:
        sichash::PartitionedSicHash<minimal, ribbonWidth, minimalFanoLowerBits> *perfectHashing = nullptr;
        sichash::SicHashConfig config;

        SicHashContender(size_t N, double loadFactor, sichash::SicHashConfig config)
                : Contender(N, minimal ? 1.0 : loadFactor), config(config) {
            this->config.loadFactor = loadFactor;
        }

        ~SicHashContender() override {
            delete perfectHashing;
        }

        std::string name() override {
            return std::string("SicHash")
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
            perfectHashing = new sichash::PartitionedSicHash
                    <minimal, ribbonWidth, minimalFanoLowerBits>(keys, config, numThreads);
        }

        size_t sizeBits() override {
            return perfectHashing->spaceUsage();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, *perfectHashing);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, *perfectHashing);
        }
};

void sicHashContenderRunner(size_t N, double loadFactor, bool minimalOnly, bool onlyPartial);
