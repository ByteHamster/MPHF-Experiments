#pragma once

#include <gpuptmphf.hpp>
#include "Contender.h"

class GpuPhobicContender : public Contender {
    public:
        float averageBucketSize;
        size_t partitionSize;
        float tradeoff;
        gpupthash::MPHFbuilder builder;
        using PilotEncoder = gpupthash::interleaved_encoder_dual<gpupthash::rice, pthash::compact>;
        using PartitionEncoder = gpupthash::diff_partition_encoder<gpupthash::compact>;
        gpupthash::MPHF<PilotEncoder, PartitionEncoder, gpupthash::xxhash> f;

        GpuPhobicContender(size_t N, float averageBucketSize, size_t partitionSize, float tradeoff)
                : Contender(N, 1.0), averageBucketSize(averageBucketSize), partitionSize(partitionSize),
                  tradeoff(tradeoff),
                  builder(gpupthash::MPHFconfig(averageBucketSize, partitionSize)) {
            f.getPilotEncoder().setEncoderTradeoff(tradeoff);
            //App::getInstance().printDebugInfo();
        }

        std::string name() override {
            return std::string("GpuPhobic")
                  + " partitionSize=" + std::to_string(partitionSize)
                  + " A=" + std::to_string(averageBucketSize)
                  + " tradeoff=" + std::to_string(tradeoff)
                  + " pilotEncoder=" + typeid(PilotEncoder).name()
                  + " partitionEncoder=" + typeid(PartitionEncoder).name();
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            builder.build(keys, f);
        }

        size_t sizeBits() override {
            return f.getBitsPerKey() * N;
        }

        void performQueries(const std::vector<std::string> &keys) override {
            doPerformQueries(keys, f);
        }

        void performTest(const std::vector<std::string> &keys) override {
            doPerformTest(keys, f);
        }
};

void gpuPhobicContenderRunner(size_t N) {
    for (float averageBucketSize = 2.0; averageBucketSize <= 12; averageBucketSize += 0.5) {
        for (float tradeoff = 0; tradeoff <= 1; tradeoff += 0.2) {
            { GpuPhobicContender(N, averageBucketSize, 2048, tradeoff).run(); }
        }
    }
}
