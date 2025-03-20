#include "FchCmphContender.h"

void fchCmphContenderRunner(size_t N, double loadFactor) {
    for (int c = 3; c < 8; c++) {
        { FchCmphContender(N, loadFactor, c, true).run(); } // Hangs
    }
}
