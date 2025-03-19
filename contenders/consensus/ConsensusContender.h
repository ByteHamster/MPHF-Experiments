#pragma once

#include "Contender.h"
#include <ConsensusRecSplit.h>

template <size_t k, double overhead>
class ConsensusContender : public Contender {
    public:
        consensus::ConsensusRecSplit<k, overhead> *consensus = nullptr;

        ConsensusContender(size_t N) : Contender(N, 1.0) {
        }

        ~ConsensusContender() override {
            delete consensus;
        }

        std::string name() override {
            return std::string("Consensus")
                + " k=" + std::to_string(k)
                + " overhead=" + std::to_string(overhead);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            consensus = new consensus::ConsensusRecSplit<k, overhead>(std::span(keys.begin(), keys.end()));
        }

        size_t sizeBits() override {
            return consensus->getBits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, *consensus);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, *consensus);
        }
};

void consensusContenderRunner(size_t N);
