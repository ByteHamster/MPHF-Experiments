#include "FiPSContender.h"

void fiPSContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 5.0; gamma += 0.15) {
        FiPSContender<512, uint32_t, false>(N, gamma).run();
        FiPSContender<256, uint16_t>(N, gamma).run();
        FiPSContender<64, uint16_t>(N, gamma).run();
    }
}
