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

void morphisHashFlatContenderRunner(size_t N);
