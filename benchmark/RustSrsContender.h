#pragma once

#include "RustContender.h"

extern "C" {
void *createSrsStruct(uint64_t len, const char **str);
void constructSrs(void *rustStruct, double overhead);
uint64_t querySrs(void *rustStruct, const char *key, const size_t length);
size_t sizeSrsBits(void *rustStruct);
void destroySrsStruct(void *rustStruct);
}

class RustSrsContender : public RustContender {
    protected:
        void *rustStruct = nullptr;
        double overhead;
    public:

        RustSrsContender(size_t N, double overhead)
                : RustContender(N), overhead(overhead) {
        }

        ~RustSrsContender() override {
            if (rustStruct != nullptr) {
                destroySrsStruct(rustStruct);
                rustStruct = nullptr;
            }
        }

        std::string name() override {
            return std::string("RustSrsContender")
                + " overhead=" + std::to_string(overhead);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            RustContender::beforeConstruction(keys);
            rustStruct = createSrsStruct(N, data);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            constructSrs(rustStruct, overhead);
        }

        size_t sizeBits() override {
            return sizeSrsBits(rustStruct);
        }

        void performQueries(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return querySrs(rustStruct, key.c_str(), key.length());
            };
            doPerformQueries(keys, x);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return querySrs(rustStruct, key.c_str(), key.length());
            };
            doPerformTest(keys, x);
        }

};

void rustSrsContenderRunner(size_t N) {
    for (double overhead = 0.1; overhead >= 0.001; overhead -= 0.005) {
        { RustSrsContender(N, overhead).run(); }
    }
}
