#include "BBHashContender.h"

void bbHashContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 5.0; gamma += 0.15) {
        for (double perc_elem_loaded = 0.05; perc_elem_loaded < 1.0; perc_elem_loaded += 0.8) {
            BBHashContender(N, gamma, perc_elem_loaded).run();
        }
        BBHashContender(N, gamma, 0).run();
    }
}