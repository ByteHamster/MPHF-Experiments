#pragma once

#include "Contender.h"
#include "RustFmphContender.h"

extern "C" {
void *createFmphGoStruct(uint64_t len, const char **str);
void constructFmphGo(void *rustStruct, uint16_t);
uint64_t queryFmphGo(void *rustStruct, const char *key, const size_t length);
size_t sizeFmphGo(void *rustStruct);
void destroyFmphGoStruct(void *rustStruct);
}

class RustFmphGoContender : public RustFmphContender {
    public:

        RustFmphGoContender(size_t N, double gamma)
            : RustFmphContender(N, gamma) {
        }

        ~RustFmphGoContender() override {
            destroyFmphGoStruct(rustStruct);
            rustStruct = nullptr;
        }

        std::string name() override {
            return std::string("RustFmphGoContender")
                + " gamma=" + std::to_string(gamma);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            std::cout << "Converting input" << std::endl;
            for (size_t i = 0; i < N; i++) {
                data[i] = keys.at(i).c_str();
            }
            std::cout << "Sending to Rust" << std::endl;
            if (!rayonThreadsInitialized) {
                rayonThreadsInitialized = true;
                initializeRayonThreadPool(numThreads);
            }
            rustStruct = createFmphGoStruct(N, data);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            constructFmphGo(rustStruct, 100 * gamma);
        }

        size_t sizeBits() override {
            return sizeFmphGo(rustStruct) * 8;
        }

        void performQueries(const std::vector<std::string> &keys) override {
            auto x = [&] (std::string &key) {
                return queryFmphGo(rustStruct, key.c_str(), key.length());
            };
            doPerformQueries(keys, x);
        }

        void performTest(const std::vector<std::string> &keys) override {
            auto x = [&] (std::string &key) {
                return queryFmphGo(rustStruct, key.c_str(), key.length());
            };
            doPerformTest(keys, x);
        }

};

void rustFmphGoContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 2.0; gamma += 0.15) {
        { RustFmphGoContender(N, gamma).run(); }
    }
}
