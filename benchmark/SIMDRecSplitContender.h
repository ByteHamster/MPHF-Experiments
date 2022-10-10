#pragma once

#include <function/SIMDRecSplit.hpp>
#include "Contender.h"

template<int l>
class SIMDRecSplitContender : public Contender {
    public:
        size_t bucketSize;
        bez::function::SIMDRecSplit<l> *recSplit = nullptr;

        SIMDRecSplitContender(size_t N, size_t bucketSize)
                : Contender(N, 1.0), bucketSize(bucketSize) {
        }

        ~SIMDRecSplitContender() override {
            delete recSplit;
        }

        std::string name() override {
            return std::string("SIMDRecSplit")
                  + " l=" + std::to_string(l)
                  + " b=" + std::to_string(bucketSize);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            recSplit = new bez::function::SIMDRecSplit<l>(keys, bucketSize, 1);
        }

        size_t sizeBits() override {
            return recSplit->getBitsPerKey() * N;
        }

        void performQueries(const std::vector<std::string> &keys) override {
            doPerformQueries(keys, *recSplit);
        }

        void performTest(const std::vector<std::string> &keys) override {
            doPerformTest(keys, *recSplit);
        }
};

template <int l>
void simdRecSplitTestMulti(size_t N) {
    {SIMDRecSplitContender<l>(N, l).run();}
    {SIMDRecSplitContender<l>(N, 2 * l).run();}
    {SIMDRecSplitContender<l>(N, 50).run();}
    {SIMDRecSplitContender<l>(N, 100).run();}
    {SIMDRecSplitContender<l>(N, 250).run();}
    {SIMDRecSplitContender<l>(N, 500).run();}
    {SIMDRecSplitContender<l>(N, 750).run();}
    {SIMDRecSplitContender<l>(N, 1000).run();}
    {SIMDRecSplitContender<l>(N, 1500).run();}
    {SIMDRecSplitContender<l>(N, 2000).run();}
}

void simdRecSplitContenderRunner(size_t N) {
    simdRecSplitTestMulti<2>(N);
    simdRecSplitTestMulti<3>(N);
    simdRecSplitTestMulti<4>(N);
    simdRecSplitTestMulti<5>(N);
    simdRecSplitTestMulti<6>(N);
    simdRecSplitTestMulti<7>(N);
    simdRecSplitTestMulti<8>(N);
}
