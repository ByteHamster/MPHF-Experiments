#pragma once

#include "Contender.h"

extern "C" {
void *createFmphStruct(uint64_t len, const char **str);
void constructFmph(void *rustStruct, uint16_t);
uint64_t queryFmph(void *rustStruct, const char *key);
size_t sizeFmph(void *rustStruct);
void destroyStruct(void *rustStruct);
}

class RustFmphContender : public Contender {
    private:
        void *rustStruct = nullptr;
        const char **data;
        double gamma;
    public:

        RustFmphContender(size_t N, double gamma)
            : Contender(N, 1.0), gamma(gamma) {
            data = static_cast<const char **>(malloc(N * sizeof(char*)));
        }

        ~RustFmphContender() override {
            destroyStruct(rustStruct);
            free(data);
        }

        std::string name() override {
            return std::string("RustFmphContender")
                + " gamma=" + std::to_string(gamma);
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
            constructFmph(rustStruct, 100 * gamma);
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
    for (double gamma = 1.0; gamma <= 2.0; gamma += 0.15) {
        { RustFmphContender(N, gamma).run(); }
    }
}
