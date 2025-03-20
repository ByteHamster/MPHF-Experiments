#pragma once

#include <function/RecSplitRotate.hpp>
#include "Contender.h"

template<int l>
class RecSplitRotateContender : public Contender {
    public:
        size_t bucketSize;
        bez::function::recsplit_rotate::RecSplit<l> *recSplit = nullptr;

        RecSplitRotateContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~RecSplitRotateContender() override {
            delete recSplit;
        }

        std::string name() override {
            return std::string("RecSplitRotate")
                  + " l=" + std::to_string(l)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            recSplit = new bez::function::recsplit_rotate::RecSplit<l>(keys, bucketSize, numThreads);
        }

        size_t sizeBits() override {
            return recSplit->getBits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, *recSplit);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, *recSplit);
        }
};

void recSplitRotateContenderRunner(size_t N);
