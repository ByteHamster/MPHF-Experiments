#include "RustFmphGoContender.h"

void rustFmphGoContenderRunner(size_t N) {
    for (double gamma = 1.0; gamma <= 5.0; gamma += 0.15) {
        { RustFmphGoContender(N, gamma).run(); }
    }
}
