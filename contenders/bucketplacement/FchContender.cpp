#include "FchContender.h"

void fchPtHashContenderRunner(size_t N) {
    for (double c = 1.0; c >= 4.0; c += 0.2) {
        FchContender(N, c).run();
    }
}
