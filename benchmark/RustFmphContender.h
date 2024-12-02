#pragma once

#include "RustContender.h"

extern "C" {
void *createFmphStruct(uint64_t len, const char **str);
void constructFmph(void *rustStruct, uint16_t);
uint64_t queryFmph(void *rustStruct, const char *key, const size_t length);
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
        }

        ~RustFmphContender() override {
            if (rustStruct != nullptr) {
                destroyFmphStruct(rustStruct);
                rustStruct = nullptr;
            }
        }

        std::string name() override {
            return std::string("RustFmphContender")
                + " gamma=" + std::to_string(gamma);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            RustContender::beforeConstruction(keys);
            rustStruct = createFmphStruct(N, data);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            constructFmph(rustStruct, 100 * gamma);
        }

        size_t sizeBits() override {
            return sizeFmph(rustStruct) * 8;
        }

        void performQueries(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryFmph(rustStruct, key.c_str(), key.length());
            };
            doPerformQueries(keys, x);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryFmph(rustStruct, key.c_str(), key.length());
            };
            doPerformTest(keys, x);
        }

};

void rustFmphContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 2.5; gamma += 0.15) {
        { RustFmphContender(N, gamma).run(); }
    }
}
