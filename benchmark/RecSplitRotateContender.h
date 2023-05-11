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
            recSplit = new bez::function::recsplit_rotate::RecSplit<l>(keys, bucketSize);
        }

        size_t sizeBits() override {
            return recSplit->getBits();
        }

        void performQueries(const std::vector<std::string> &keys) override {
            doPerformQueries(keys, *recSplit);
        }

        void performTest(const std::vector<std::string> &keys) override {
            doPerformTest(keys, *recSplit);
        }
};

template <int l>
void recSplitRotateTestMulti(size_t N) {
    {RecSplitRotateContender<l>(N, l).run();}
    {RecSplitRotateContender<l>(N, 50).run();}
    {RecSplitRotateContender<l>(N, 100).run();}
    {RecSplitRotateContender<l>(N, 500).run();}
    {RecSplitRotateContender<l>(N, 2000).run();}
}

void recSplitRotateContenderRunner(size_t N) {
    recSplitRotateTestMulti<4>(N);
    recSplitRotateTestMulti<5>(N);
    recSplitRotateTestMulti<6>(N);
    recSplitRotateTestMulti<7>(N);
    recSplitRotateTestMulti<8>(N);
    recSplitRotateTestMulti<11>(N);
    recSplitRotateTestMulti<12>(N);
    recSplitRotateTestMulti<13>(N);
    recSplitRotateTestMulti<14>(N);
}
