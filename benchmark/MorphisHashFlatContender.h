#pragma once

#include "Contender.h"
#include <MorphisHashFlat.h>

template<int l, int eb, int ws>
class MorphisHashFlatContender : public Contender {
    public:
        morphishash::MorphisHashFlat<l, ws, eb> *morphisHashFlat = nullptr;

        MorphisHashFlatContender(size_t N)
                : Contender(N, 1.0) {
        }

        ~MorphisHashFlatContender() override {
            delete morphisHashFlat;
        }

        std::string name() override {
            return std::string("MorphisHashFlat")
                + " eb=" + std::to_string(eb)
                + " ws=" + std::to_string(ws)
                + " l=" + std::to_string(l);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            (void) keys;
        }

        void construct(const std::vector<std::string> &keys) override {
            morphisHashFlat = new morphishash::MorphisHashFlat<l, ws, eb>(keys);
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

template <int l, int e>
void morphisHashFlatContenderRunnerMultiWidth(size_t N) {
    {MorphisHashFlatContender<l, e, 1>(N).run();}
    {MorphisHashFlatContender<l, e, 2>(N).run();}
    {MorphisHashFlatContender<l, e, 3>(N).run();}
    {MorphisHashFlatContender<l, e, 4>(N).run();}
    {MorphisHashFlatContender<l, e, 5>(N).run();}
    {MorphisHashFlatContender<l, e, 6>(N).run();}
}

template <int l>
void morphisHashFlatContenderRunnerMultiExtraBit(size_t N) {
    morphisHashFlatContenderRunnerMultiWidth<l, 2>(N);
    morphisHashFlatContenderRunnerMultiWidth<l, 3>(N);
    morphisHashFlatContenderRunnerMultiWidth<l, 4>(N);
    morphisHashFlatContenderRunnerMultiWidth<l, 5>(N);
}

void morphisHashFlatContenderRunner(size_t N) {
    morphisHashFlatContenderRunnerMultiExtraBit<28>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<34>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<40>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<46>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<52>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<58>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<64>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<70>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<76>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<82>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<88>(N);
}