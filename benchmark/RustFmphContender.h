#pragma once

#include "Contender.h"

extern "C" {
void *createFmphStruct(uint64_t len, const char **str);
void constructFmph(void *rustStruct);
uint64_t queryFmph(void *rustStruct, const char *key);
size_t sizeFmph(void *rustStruct);
void destroyStruct(void *rustStruct);
}

class RustFmphContender : public Contender {
    private:
        void *rustStruct = nullptr;
        const char **data;
    public:

        RustFmphContender(size_t N)
            : Contender(N, 1.0) {
            data = static_cast<const char **>(malloc(N * sizeof(char*)));
        }

        ~RustFmphContender() override {
            destroyStruct(rustStruct);
            free(data);
        }

        std::string name() override {
            return std::string("RustContender");
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            std::cout << "Converting input" << std::endl;
            for (size_t i = 0; i < N; i++) {
                data[i] = keys.at(i).c_str();
            }
            std::cout << "Sending to Rust" << std::endl;
            rustStruct = createFmphStruct(N, data);
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            constructFmph(rustStruct);
        }

        size_t sizeBits() override {
            return sizeFmph(rustStruct) * 8;
        }

        void performQueries(const std::vector<std::string> &keys) override {
            auto x = [&] (std::string &key) {
                return queryFmph(rustStruct, key.c_str());
            };
            doPerformQueries(keys, x);
        }

        void performTest(const std::vector<std::string> &keys) override {
            auto x = [&] (std::string &key) {
                return queryFmph(rustStruct, key.c_str());
            };
            doPerformTest(keys, x);
        }

};

void rustFmphContenderRunner(size_t N) {
    {RustFmphContender(N).run();}
}
