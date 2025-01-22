#pragma once

#include "Contender.h"
#include <MorphisHashFlat.h>

template<int l, int ws>
class MorphisHashFlatContender : public Contender {
    public:
        morphishash::MorphisHashFlat<l, ws> *morphisHashFlat = nullptr;

        MorphisHashFlatContender(size_t N)
                : Contender(N, 1.0) {
        }

        ~MorphisHashFlatContender() override {
            delete morphisHashFlat;
        }

        std::string name() override {
            return std::string("MorphisHashFlat")
                + " ws=" + std::to_string(ws)
                + " l=" + std::to_string(l);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            morphisHashFlat = new morphishash::MorphisHashFlat<l, ws>(keys);
        }

        size_t sizeBits() override {
            return morphisHashFlat->getBits();
        }

        void performQueries(const std::span<std::string> keys) override {
            doPerformQueries(keys, *morphisHashFlat);
        }

        void performTest(const std::span<std::string> keys) override {
            doPerformTest(keys, *morphisHashFlat);
        }
};

template <int l>
void morphisHashFlatContenderRunnerMultiWidth(size_t N) {
    {MorphisHashFlatContender<l, 1>(N).run();}
    {MorphisHashFlatContender<l, 2>(N).run();}
    {MorphisHashFlatContender<l, 3>(N).run();}
    {MorphisHashFlatContender<l, 4>(N).run();}
    {MorphisHashFlatContender<l, 5>(N).run();}
    {MorphisHashFlatContender<l, 6>(N).run();}
    {MorphisHashFlatContender<l, 7>(N).run();}
}

void morphisHashFlatContenderRunner(size_t N) {
    morphisHashFlatContenderRunnerMultiWidth<34>(N);
    morphisHashFlatContenderRunnerMultiWidth<42>(N);
    morphisHashFlatContenderRunnerMultiWidth<50>(N);
    morphisHashFlatContenderRunnerMultiWidth<58>(N);
    morphisHashFlatContenderRunnerMultiWidth<64>(N);
    morphisHashFlatContenderRunnerMultiWidth<72>(N);
    morphisHashFlatContenderRunnerMultiWidth<80>(N);
    morphisHashFlatContenderRunnerMultiWidth<88>(N);
    morphisHashFlatContenderRunnerMultiWidth<96>(N);
}