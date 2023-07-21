#pragma once

#include "Contender.h"

extern "C" {
void *createStruct(uint64_t len, char **str);
void *useStruct(void *rustStruct);
void *destroyStruct(void *rustStruct);
}

class RustContender : public Contender {
    private:
        void *rustStruct = nullptr;
    public:

        RustContender(size_t N)
            : Contender(N, 1.0) {
        }

        ~RustContender() override {
            destroyStruct(rustStruct);
        }

        std::string name() override {
            return std::string("RustContender");
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            char * array[2];
            char *x = "aaaa";
            char *y = "bbbb";
            array[0] = x;
            array[1] = y;
            char **ptr = array;
            rustStruct = createStruct(2, ptr);
        }

        void construct(const std::vector<std::string> &keys) override {
            useStruct(rustStruct);
        }

        size_t sizeBits() override {
            return 0;
        }

        void performQueries(const std::vector<std::string> &keys) override {
        }

        void performTest(const std::vector<std::string> &keys) override {
        }

};
