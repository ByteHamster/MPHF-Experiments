#pragma once

#include "RustContender.h"

extern "C" {
    void *createPhastStruct();
    void constructPhast(void *rustStruct, void *keysStruct, uint8_t bits_per_seed, uint16_t bucket_size100, size_t threads);
    uint64_t queryPhast(void *rustStruct, const char *key, size_t length);
    void queryPhastAll(void *rustStruct, void *keysStruct);
    size_t sizePhast(void *rustStruct);
    void destroyPhastStruct(void *rustStruct);
}

class RustPhastContender : public RustContender {
    protected:
        void *rustStruct = nullptr;
        uint8_t bits_per_seed;
        uint16_t bucket_size100;
    public:
        RustPhastContender(size_t N, uint8_t bits_per_seed, uint16_t bucket_size100)
            : RustContender(N, RUST_INPUT_VEC_SLICE), bits_per_seed(bits_per_seed), bucket_size100(bucket_size100) {
            rustStruct = createPhastStruct();
        }

        ~RustPhastContender() override {
            destroyPhastStruct(rustStruct);
        }

        std::string name() override {
            return std::string("RustPHast")
                + " bits_per_seed=" + std::to_string(bits_per_seed)
                + " bucket_size100=" + std::to_string(bucket_size100);
        }

        void construct(void *keys) override {
            constructPhast(rustStruct, keys, bits_per_seed, bucket_size100, numThreads);
        }

        size_t sizeBits() override {
            return sizePhast(rustStruct) * 8;
        }

        void performQueries(void *keys) override {
            queryPhastAll(rustStruct, keys);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryPhast(rustStruct, key.c_str(), key.length()); };
            doPerformTest(keys, x);
        }
};

void rustPHastContenderRunner(size_t N);
