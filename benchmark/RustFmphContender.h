#pragma once

#include "Contender.h"

extern "C" {
void *createQueryPlanStruct(uint64_t len, const char **str);
void destroyQueryPlanStruct(void *rustStruct);

void *createFmphStruct(uint64_t len, const char **str);
void constructFmph(void *rustStruct, uint16_t);
uint64_t queryFmph(void *rustStruct, const char *key);
size_t sizeFmph(void *rustStruct);
void destroyFmphStruct(void *rustStruct);
uint64_t queryMultiFmph(void *rustStruct, void *queryPlan);
}

class RustFmphContender : public Contender {
    protected:
        void *rustStruct = nullptr;
        void *queryPlanStruct = nullptr;
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
            if (queryPlanStruct != nullptr) {
                destroyQueryPlanStruct(queryPlanStruct);
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

        void prepareQueries(const std::vector<std::string> &keys) override {
            std::cout << "Converting query plan" << std::endl;
            for (size_t i = 0; i < keys.size(); i++) {
                data[i] = keys.at(i).c_str();
            }
            std::cout << "Sending to Rust" << std::endl;
            queryPlanStruct = createQueryPlanStruct(N, data);
        }

        void performQueries(const std::vector<std::string> &keys) override {
            (void) keys;
            uint64_t x = queryMultiFmph(rustStruct, queryPlanStruct);
            DO_NOT_OPTIMIZE(x);
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
