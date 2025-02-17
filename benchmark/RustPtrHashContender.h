#pragma once

#include "RustContender.h"

extern "C" {
void *createPtrHashStruct(uint64_t len, const char **str);
void constructPtrHash(void *rustStruct, bool compact);
uint64_t queryPtrHash(void *rustStruct, const char *key, const size_t length);
size_t sizePtrHash(void *rustStruct);
void destroyPtrHashStruct(void *rustStruct);
}

class RustPtrHashContender : public RustContender {
    protected:
        void *rustStruct = nullptr;
        bool compact;
    public:

        RustPtrHashContender(size_t N, bool compact)
            : RustContender(N), compact(compact) {
        }

        ~RustPtrHashContender() override {
            if (rustStruct != nullptr) {
                destroyPtrHashStruct(rustStruct);
                rustStruct = nullptr;
            }
        }

        std::string name() override {
            return std::string("RustPtrHash")
                + " compact=" + std::to_string(compact);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            RustContender::beforeConstruction(keys);
            rustStruct = createPtrHashStruct(N, data);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            constructPtrHash(rustStruct, compact);
        }

        size_t sizeBits() override {
            return sizePtrHash(rustStruct) * 8;
        }

        void performQueries(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryPtrHash(rustStruct, key.c_str(), key.length());
            };
            doPerformQueries(keys, x);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return queryPtrHash(rustStruct, key.c_str(), key.length());
            };
            doPerformTest(keys, x);
        }

};

void rustPtrHashContenderRunner(size_t N) {
    { RustPtrHashContender(N, true).run(); }
    { RustPtrHashContender(N, false).run(); }
}
