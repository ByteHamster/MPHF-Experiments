#include "ChmContender.h"

void chmContenderRunner(size_t N, double loadFactor) {
    {ChmContender(N, loadFactor, loadFactor, true).run();}
}
