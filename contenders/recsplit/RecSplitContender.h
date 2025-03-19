#pragma once

#include <sux/function/RecSplit.hpp>
#include "Contender.h"

template<int l>
class RecSplitContender : public Contender {
    public:
        size_t bucketSize;
        sux::function::RecSplit<l> *recSplit = nullptr;

        RecSplitContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~RecSplitContender() override {
            delete recSplit;
        }

        std::string name() override {
            return std::string("RecSplit")
                  + " l=" + std::to_string(l)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            recSplit = new sux::function::RecSplit<l>(keys, bucketSize);
        }

        size_t sizeBits() override {
            return recSplit->bitCount();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, *recSplit);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, *recSplit);
        }
};

void recSplitContenderRunner(size_t N);
