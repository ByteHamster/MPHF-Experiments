#pragma once

#include "Contender.h"

extern "C" {
static bool rayonThreadsInitialized = false;
void initializeRayonThreadPool(uint64_t threads);
}

class RustContender : public Contender {
    protected:
        const char **data;
    public:

        RustContender(size_t N)
            : Contender(N, 1.0) {
            data = static_cast<const char **>(malloc(std::max(N, Contender::numQueries) * sizeof(char*)));
        }

        ~RustContender() override {
            free(data);
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
        }
};
