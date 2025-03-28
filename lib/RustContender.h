#pragma once

#include "Contender.h"

extern "C" {
static bool rayonThreadsInitialized = false;
void initializeRayonThreadPool(uint64_t threads);
}

class RustContender : public Contender {
    protected:
        const char **keysAsCString;
    public:

        RustContender(size_t N)
            : Contender(N, 1.0) {
            keysAsCString = static_cast<const char **>(malloc(std::max(N, Contender::numQueries) * sizeof(char*)));
        }

        ~RustContender() override {
            free(keysAsCString);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            std::cout << "Converting input" << std::endl;
            for (size_t i = 0; i < N; i++) {
                keysAsCString[i] = keys[i].c_str();
            }
            std::cout << "Sending to Rust" << std::endl;
            if (!rayonThreadsInitialized) {
                rayonThreadsInitialized = true;
                initializeRayonThreadPool(numThreads);
            }
        }

        void beforeQueries(const std::span<std::string> &keys) override {
            std::cout << "Converting input" << std::endl;
            for (size_t i = 0; i < N; i++) {
                keysAsCString[i] = keys[i].c_str();
            }
            std::cout << "Sending to Rust" << std::endl;
        }
};
