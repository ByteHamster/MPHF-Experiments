#include "BdzContender.h"

void bdzContenderRunner(size_t N, double loadFactor) {
    for (int b = 1; b < 8; b++) {
        if (loadFactor <= 0.8) {BdzContender(N, loadFactor, 1.0/loadFactor, b, true).run();} // b=number of bits of k
    }
    if (loadFactor <= 0.8) {BdzContender(N, loadFactor, 1.0/loadFactor, 0, false).run();} // b ignored
}
