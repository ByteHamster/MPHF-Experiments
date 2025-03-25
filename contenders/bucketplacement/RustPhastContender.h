#pragma once

#include "RustContender.h"

extern "C" {
    void *createPhastStruct(uint64_t len, const char **str);
    void constructPhast(void *rustStruct, uint8_t bits_per_seed, uint16_t bucket_size100, size_t threads);
    uint64_t queryPhast(void *rustStruct, const char *key, size_t length);
    uint64_t queryPhast8(void *rustStruct, const char *key, size_t length);
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
            : RustContender(N), bits_per_seed(bits_per_seed), bucket_size100(bucket_size100) {
        }

        ~RustPhastContender() override {
            if (rustStruct != nullptr) {
                destroyPhastStruct(rustStruct);
                rustStruct = nullptr;
            }
        }

        std::string name() override {
            return std::string("RustPHast")
                + " bits_per_seed=" + std::to_string(bits_per_seed)
                + " bucket_size100=" + std::to_string(bucket_size100);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            RustContender::beforeConstruction(keys);
            rustStruct = createPhastStruct(N, data);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            constructPhast(rustStruct, bits_per_seed, bucket_size100, numThreads);
        }

        size_t sizeBits() override {
            return sizePhast(rustStruct) * 8;
        }

        void performQueries(const std::span<std::string> keys) override {
            if (bits_per_seed == 8) {
                auto x = [&] (std::string &key) {
                    return queryPhast8(rustStruct, key.c_str(), key.length()); };
                doPerformQueries(keys, x);
            } else {
                auto x = [&] (std::string &key) {
                    return queryPhast(rustStruct, key.c_str(), key.length()); };
                doPerformQueries(keys, x);
            }
        }

        void performTest(const std::span<std::string> keys) override {
            if (bits_per_seed == 8) {
                auto x = [&] (std::string &key) {
                    return queryPhast8(rustStruct, key.c_str(), key.length()); };
                doPerformTest(keys, x);
            } else {
                auto x = [&] (std::string &key) {
                    return queryPhast(rustStruct, key.c_str(), key.length()); };
                doPerformTest(keys, x);
            }
        }
};

void rustPHastContenderRunner(size_t N);