#pragma once

#include "Contender.h"

extern "C" {
void *createFmphStruct(uint64_t len, const char **str);
void constructFmph(void *rustStruct, uint16_t);
uint64_t queryFmph(void *rustStruct, const char *key, const size_t length);
size_t sizeFmph(void *rustStruct);
void destroyFmphStruct(void *rustStruct);
static bool rayonThreadsInitialized = false;
void initializeRayonThreadPool(uint64_t threads);
}

class RustFmphContender : public Contender {
    protected:
        void *rustStruct = nullptr;
        const char **data;
        double gamma;
    public:

        RustFmphContender(size_t N, double gamma)
            : Contender(N, 1.0), gamma(gamma) {
            data = static_cast<const char **>(malloc(std::max(N, Contender::numQueries) * sizeof(char*)));
        }

        ~RustFmphContender() override {
            if (rustStruct != nullptr) {
                destroyFmphStruct(rustStruct);
            }
            free(data);
        }

        std::string name() override {
            return std::string("RustFmphContender")
                + " gamma=" + std::to_string(gamma);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            std::cout << "Converting input" << std::endl;
            for (size_t i = 0; i < N; i++) {
                data[i] = keys[i].c_str();
            }
            std::cout << "Sending to Rust" << std::endl;
            if (!rayonThreadsInitialized) {
                rayonThreadsInitialized = true;
                initializeRayonThreadPool(numThreads);
            }
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
