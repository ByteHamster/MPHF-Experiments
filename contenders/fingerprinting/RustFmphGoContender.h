#pragma once

#include "RustContender.h"

extern "C" {
void *createFmphGoStruct(uint64_t len, const char **str);
void constructFmphGo(void *rustStruct, uint16_t);
uint64_t queryFmphGo(void *rustStruct, const char *key, const size_t length);
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
        }

        ~RustFmphGoContender() override {
            if (rustStruct != nullptr) {
                destroyFmphGoStruct(rustStruct);
                rustStruct = nullptr;
            }
        }

        std::string name() override {
            return std::string("RustFmphGoContender")
                + " gamma=" + std::to_string(gamma);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            RustContender::beforeConstruction(keys);
            rustStruct = createFmphGoStruct(N, data);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            constructFmphGo(rustStruct, 100 * gamma);
        }

        size_t sizeBits() override {
            return sizeFmphGo(rustStruct) * 8;
        }

        void performQueries(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryFmphGo(rustStruct, key.c_str(), key.length());
            };
            doPerformQueries(keys, x);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryFmphGo(rustStruct, key.c_str(), key.length());
            };
            doPerformTest(keys, x);
        }

};

void rustFmphGoContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 5.0; gamma += 0.15) {
        { RustFmphGoContender(N, gamma).run(); }
    }
}
