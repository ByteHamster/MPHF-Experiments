#pragma once

#include "Contender.h"

extern "C" {
static bool rayonThreadsInitialized = false;
void initializeRayonThreadPool(uint64_t threads);
void *convertToVecString(uint64_t len, const char **str);
void destroyVecString(void *rustStruct);
void *convertToVecSlice(uint64_t len, const char **str);
void destroyVecSlice(void *rustStruct);
}

class RustContender : public Contender {
    public:
        static constexpr int RUST_INPUT_VEC_STRING = 1;
        static constexpr int RUST_INPUT_VEC_SLICE = 2;
    private:
        const char **keysAsCString;
        void *keysRustWrapper = nullptr;
        int rustInputType = 0;
    public:
        RustContender(size_t N, int rustInputType)
                : Contender(N, 1.0), rustInputType(rustInputType) {
            keysAsCString = static_cast<const char **>(malloc(std::max(N, Contender::numQueries) * sizeof(char*)));
        }

        ~RustContender() override {
            free(keysAsCString);
            freeKeysWrapper();
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            std::cout << "Converting input for Rust" << std::endl;
            for (size_t i = 0; i < keys.size(); i++) {
                keysAsCString[i] = keys[i].c_str();
            }
            freeKeysWrapper();
            if (rustInputType == RUST_INPUT_VEC_STRING) {
                keysRustWrapper = convertToVecString(keys.size(), keysAsCString);
            } else {
                keysRustWrapper = convertToVecSlice(keys.size(), keysAsCString);
            }
            if (!rayonThreadsInitialized) {
                rayonThreadsInitialized = true;
                //initializeRayonThreadPool(numThreads); //TODO: Make sure to set number of threads
            }
        }

        void construct(const std::vector<std::string> &keys) final {
            (void) keys;
            construct(keysRustWrapper);
        }

        virtual void construct(void *keysRustWrapper) = 0;

        void beforeQueries(const std::span<std::string> &keys) override {
            std::cout << "Converting input for Rust" << std::endl;
            for (size_t i = 0; i < keys.size(); i++) {
                keysAsCString[i] = keys[i].c_str();
            }
            freeKeysWrapper();
            if (rustInputType == RUST_INPUT_VEC_STRING) {
                keysRustWrapper = convertToVecString(keys.size(), keysAsCString);
            } else {
                keysRustWrapper = convertToVecSlice(keys.size(), keysAsCString);
            }
        }

        void performQueries(const std::span<std::string> keys) final {
            (void) keys;
            performQueries(keysRustWrapper);
        }

        virtual void performQueries(void *keysRustWrapper) = 0;

    private:
        void freeKeysWrapper() {
            if (keysRustWrapper == nullptr) {
                return;
            }
            if (rustInputType == RUST_INPUT_VEC_STRING) {
                destroyVecString(keysRustWrapper);
            } else {
                destroyVecSlice(keysRustWrapper);
            }
            keysRustWrapper = nullptr;
        }
};
