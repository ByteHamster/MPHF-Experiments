#include "RustFmphContender.h"

void rustFmphContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 5.0; gamma += 0.15) {
        { RustFmphContender(N, gamma).run(); }
    }
}
