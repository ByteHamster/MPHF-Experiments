#pragma once

#include <cmph.h>

#include <utility>
#undef MAX_BUCKET_SIZE
#include "Contender.h"

class FchCmphContender : public Contender {
    public:
        cmph_t *mphf = nullptr;
        cmph_io_adapter_t *source = nullptr;
        const char **data;
        double c;

        FchCmphContender(size_t N, double loadFactor, double c, bool minimal)
                : Contender(N, minimal ? 1.0 : loadFactor), c(c) {
            data = static_cast<const char **>(malloc(N * sizeof(char*)));
        }

        ~FchCmphContender() override {
            if (mphf != nullptr) {
                cmph_destroy(mphf);
            }
            free(source);
            free(data);
        }

        std::string name() override {
            return std::string("cmph-FCH")
                    + " c=" + std::to_string(c);
        }

        void beforeConstruction(const std::vector<std::string> &keys) override {
            std::cout << "Converting input" << std::endl;
            for (size_t i = 0; i < N; i++) {
                data[i] = keys[i].c_str();
            }
            source = cmph_io_vector_adapter((char **)data, N); // They even do the const cast in their readme file
        }

        void construct(const std::vector<std::string> &keys) override {
            (void) keys;
            //Create mphf
            cmph_config_t *config = cmph_config_new(source);
            cmph_config_set_algo(config, CMPH_FCH);
            cmph_config_set_verbosity(config, 0);
            cmph_config_set_graphsize(config, c);
            //cmph_config_set_b(config, b);
            mphf = cmph_new(config);

            cmph_config_destroy(config);
            if (mphf == nullptr) {
                throw std::logic_error("Unable to create minimum perfect hashing function");
            }
        }

        size_t sizeBits() override {
            return 8 * cmph_packed_size(mphf);
        }

        void performQueries(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return cmph_search(mphf, key.c_str(), key.length());
            };
            doPerformQueries(keys, x);
        }

        void performTest(const std::span<std::string> keys) override {
            auto x = [&] (std::string &key) {
                return cmph_search(mphf, key.c_str(), key.length());
            };
            doPerformTest(keys, x);
        }
};

void fchCmphContenderRunner(size_t N, double loadFactor);
