#pragma once

#include "RustContender.h"

extern "C" {
    void *createPtrHashStruct(uint64_t len, const char **str);
    void constructPtrHash(void *rustStruct, uint64_t variant, double lambda);
    uint64_t queryPtrHash1(void *rustStruct, const char *key, size_t length);
    uint64_t queryPtrHash2(void *rustStruct, const char *key, size_t length);
    uint64_t queryPtrHash3(void *rustStruct, const char *key, size_t length);
    uint64_t queryPtrHash4(void *rustStruct, const char *key, size_t length);
    uint64_t queryPtrHash5(void *rustStruct, const char *key, size_t length);
    uint64_t queryPtrHash6(void *rustStruct, const char *key, size_t length);
    size_t sizePtrHash(void *rustStruct);
    void destroyPtrHashStruct(void *rustStruct);
}

class RustPtrHashContender : public RustContender {
    protected:
        void *rustStruct = nullptr;
        uint64_t variant;
        double lambda;
    public:

        RustPtrHashContender(size_t N, uint64_t variant, double lambda)
            : RustContender(N), variant(variant), lambda(lambda) {
        }

        ~RustPtrHashContender() override {
            if (rustStruct != nullptr) {
                destroyPtrHashStruct(rustStruct);
                rustStruct = nullptr;
            }
        }

        std::string name() override {
            return std::string("RustPtrHash")
                + " variant=" + std::to_string(variant)
                + " lambda=" + std::to_string(lambda);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            RustContender::beforeConstruction(keys);
            rustStruct = createPtrHashStruct(N, data);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            constructPtrHash(rustStruct, variant, lambda);
        }

        size_t sizeBits() override {
            return sizePtrHash(rustStruct) * 8;
        }

        void performQueries(const std::span<std::string> keys) override {
            if (variant == 1) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash1(rustStruct, key.c_str(), key.length()); };
                doPerformQueries(keys, x);
            } else if (variant == 2) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash2(rustStruct, key.c_str(), key.length()); };
                doPerformQueries(keys, x);
            } else if (variant == 3) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash3(rustStruct, key.c_str(), key.length()); };
                doPerformQueries(keys, x);
            } else if (variant == 4) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash4(rustStruct, key.c_str(), key.length()); };
                doPerformQueries(keys, x);
            } else if (variant == 5) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash5(rustStruct, key.c_str(), key.length()); };
                doPerformQueries(keys, x);
            } else if (variant == 6) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash6(rustStruct, key.c_str(), key.length()); };
                doPerformQueries(keys, x);
            } else {
                std::cerr << "Invalid variant" << std::endl;
            }
        }

        void performTest(const std::span<std::string> keys) override {
            if (variant == 1) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash1(rustStruct, key.c_str(), key.length()); };
                doPerformTest(keys, x);
            } else if (variant == 2) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash2(rustStruct, key.c_str(), key.length()); };
                doPerformTest(keys, x);
            } else if (variant == 3) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash3(rustStruct, key.c_str(), key.length()); };
                doPerformTest(keys, x);
            } else if (variant == 4) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash4(rustStruct, key.c_str(), key.length()); };
                doPerformTest(keys, x);
            } else if (variant == 5) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash5(rustStruct, key.c_str(), key.length()); };
                doPerformTest(keys, x);
            } else if (variant == 6) {
                auto x = [&] (std::string &key) {
                    return queryPtrHash6(rustStruct, key.c_str(), key.length()); };
                doPerformTest(keys, x);
            } else {
                std::cerr << "Invalid variant" << std::endl;
            }
        }
};

void rustPtrHashContenderRunner(size_t N) {
    // Cubic bucket assignment
    for (double lambda = 2.6; lambda <= 4.01; lambda += 0.2) {
        RustPtrHashContender(N, 3, lambda).run();
        RustPtrHashContender(N, 6, lambda).run();
    }
    for (double lambda = 2.6; lambda <= 3.41; lambda += 0.2) {
        // Linear bucket assignment
        RustPtrHashContender(N, 1, lambda).run();
        RustPtrHashContender(N, 4, lambda).run();
        // Quadratic bucket assignment
        RustPtrHashContender(N, 2, lambda).run();
        RustPtrHashContender(N, 5, lambda).run();
    }
}
