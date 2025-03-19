#include "FchContender.h"

void fchPtHashContenderRunner(size_t N) {
    for (double c = 4.0; c >= 2.5; c -= 0.1) {
        FchContender(N, c).run();
    }
}
