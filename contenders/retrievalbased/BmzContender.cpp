#include "BmzContender.h"

void bmzContenderRunner(size_t N, double loadFactor) {
    {BmzContender(N, loadFactor, loadFactor, true).run();}
}
