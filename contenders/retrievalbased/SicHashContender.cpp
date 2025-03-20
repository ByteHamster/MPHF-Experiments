#include "SicHashContender.h"

template <size_t ribbonWidth>
void sicHashContenderRunnerW(size_t N, double loadFactor, bool minimalOnly) {
    for (float spaceBudget = 1.35; spaceBudget < 3.0; spaceBudget += 0.03) {
        for (float x = 0.0; x < 1.0; x += 0.2) {
            if (!minimalOnly) {
                {SicHashContender<false, ribbonWidth>(N, loadFactor, sichash::SicHashConfig().spaceBudget(spaceBudget, x)).run();}
            }

            if (loadFactor < 0.89) {
                {SicHashContender<true, ribbonWidth, 3>(N, loadFactor, sichash::SicHashConfig().spaceBudget(spaceBudget, x)).run();}
            } else if (loadFactor < 0.94) {
                {SicHashContender<true, ribbonWidth, 4>(N, loadFactor, sichash::SicHashConfig().spaceBudget(spaceBudget, x)).run();}
            } else {
                {SicHashContender<true, ribbonWidth, 5>(N, loadFactor, sichash::SicHashConfig().spaceBudget(spaceBudget, x)).run();}
            }
        }
    }
}

void sicHashContenderRunner(size_t N, double loadFactor, bool minimalOnly, bool onlyPartial) {
    sicHashContenderRunnerW<64>(N, loadFactor, minimalOnly);
    if (!onlyPartial) {
        sicHashContenderRunnerW<32>(N, loadFactor, minimalOnly);
    }
}
