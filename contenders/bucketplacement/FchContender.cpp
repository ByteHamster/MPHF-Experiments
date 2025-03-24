#include "FchContender.h"

void fchPtHashContenderRunner(size_t N) {
    for (double c = 5.0; c >= 2.0; c -= 0.2) {
        FchContender(N, c).run();
    }
}
