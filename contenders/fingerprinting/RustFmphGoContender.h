#pragma once

#include "RustContender.h"

extern "C" {
void *createFmphGoStruct();
void constructFmphGo(void *rustStruct, void *keys, uint16_t gamma);
uint64_t queryFmphGo(void *rustStruct, const char *key, size_t length);
void queryFmphGoAll(void *rustStruct, void *keys);
size_t sizeFmphGo(void *rustStruct);
void destroyFmphGoStruct(void *rustStruct);
}

class RustFmphGoContender : public RustContender {
    protected:
        void *rustStruct = nullptr;
        double gamma;
    public:

        RustFmphGoContender(size_t N, double gamma)
            : RustContender(N), gamma(gamma) {
            rustStruct = createFmphGoStruct();
        }

        ~RustFmphGoContender() override {
            destroyFmphGoStruct(rustStruct);
        }

        std::string name() override {
            return std::string("RustFmphGoContender")
                + " gamma=" + std::to_string(gamma);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            RustContender::beforeConstruction(keys);
        }

        void construct(void *keys) override {
            constructFmphGo(rustStruct, keys, 100 * gamma);
        }

        size_t sizeBits() override {
            return sizeFmphGo(rustStruct) * 8;
        }

        void performQueries(void *keys) override {
            queryFmphGoAll(rustStruct, keys);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryFmphGo(rustStruct, key.c_str(), key.length());
            };
            doPerformTest(keys, x);
        }

};

void rustFmphGoContenderRunner(size_t N);
