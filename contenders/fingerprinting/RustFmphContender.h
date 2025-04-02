#pragma once

#include "RustContender.h"

extern "C" {
void *createFmphStruct();
void constructFmph(void *rustStruct, void *keysStruct, uint16_t gamma);
uint64_t queryFmph(void *rustStruct, const char *key, size_t length);
void queryFmphAll(void *rustStruct, void *keysStruct);
size_t sizeFmph(void *rustStruct);
void destroyFmphStruct(void *rustStruct);
}

class RustFmphContender : public RustContender {
    protected:
        void *rustStruct = nullptr;
        double gamma;
    public:

        RustFmphContender(size_t N, double gamma)
            : RustContender(N), gamma(gamma) {
            rustStruct = createFmphStruct();
        }

        ~RustFmphContender() override {
            destroyFmphStruct(rustStruct);
        }

        std::string name() override {
            return std::string("RustFmphContender")
                + " gamma=" + std::to_string(gamma);
        }

        void construct(void *keys) override {
            constructFmph(rustStruct, keys, 100 * gamma);
        }

        size_t sizeBits() override {
            return sizeFmph(rustStruct) * 8;
        }

        void performQueries(void *keys) override {
            queryFmphAll(rustStruct, keys);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryFmph(rustStruct, key.c_str(), key.length());
            };
            doPerformTest(keys, x);
        }

};

void rustFmphContenderRunner(size_t N);
