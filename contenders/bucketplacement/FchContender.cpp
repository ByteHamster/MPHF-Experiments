#include "FchContender.h"

void fchPtHashContenderRunner(size_t N) {
    for (double c = 7.0; c >= 2.4; c -= 0.2) {
        FchContender(N, c).run();
    }
}
