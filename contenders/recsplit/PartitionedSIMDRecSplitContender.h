#pragma once

#include <function/PartitionedSIMDRecSplit.h>
#include "Contender.h"

template<int l>
class PartitionedSIMDRecSplitContender : public Contender {
    public:
        size_t bucketSize;
        bez::function::PartitionedSIMDRecSplit<l> *recSplit = nullptr;

        PartitionedSIMDRecSplitContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~PartitionedSIMDRecSplitContender() override {
            delete recSplit;
        }

        std::string name() override {
            return std::string("PartitionedSIMDRecSplit")
                  + " l=" + std::to_string(l)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            recSplit = new bez::function::PartitionedSIMDRecSplit<l>(keys, bucketSize, numThreads);
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

template <int l>
void partitionedSimdRecSplitTestMulti(size_t N) {
    {PartitionedSIMDRecSplitContender<l>(N, l).run();}
    {PartitionedSIMDRecSplitContender<l>(N, 50).run();}
    {PartitionedSIMDRecSplitContender<l>(N, 100).run();}
    {PartitionedSIMDRecSplitContender<l>(N, 500).run();}
    {PartitionedSIMDRecSplitContender<l>(N, 2000).run();}
}

void partitionedSimdRecSplitContenderRunner(size_t N) {
    partitionedSimdRecSplitTestMulti<4>(N);
    partitionedSimdRecSplitTestMulti<5>(N);
    partitionedSimdRecSplitTestMulti<6>(N);
    partitionedSimdRecSplitTestMulti<7>(N);
    partitionedSimdRecSplitTestMulti<8>(N);
    partitionedSimdRecSplitTestMulti<9>(N);
    partitionedSimdRecSplitTestMulti<10>(N);
    partitionedSimdRecSplitTestMulti<11>(N);
    partitionedSimdRecSplitTestMulti<12>(N);
    partitionedSimdRecSplitTestMulti<13>(N);
    partitionedSimdRecSplitTestMulti<14>(N);
    partitionedSimdRecSplitTestMulti<15>(N);
    partitionedSimdRecSplitTestMulti<16>(N);
}
