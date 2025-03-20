#include "PTHashContender.h"

void ptHashContenderRunner(size_t N, double loadFactor, bool minimalOnly) {
    for (double lambda = 3.0; lambda < 10.0; lambda += 0.4) {
        if (!minimalOnly) {
            PTHashContender<false, pthash::elias_fano>(N, loadFactor, lambda).run();
            PTHashContender<false, pthash::dictionary_dictionary>(N, loadFactor, lambda).run();
            PTHashContender<false, pthash::dictionary_elias_fano>(N, loadFactor, lambda).run();
        }

        PTHashContender<true, pthash::elias_fano>(N, loadFactor, lambda).run();
        PTHashContender<true, pthash::dictionary_dictionary>(N, loadFactor, lambda).run();
        PTHashContender<true, pthash::dictionary_elias_fano>(N, loadFactor, lambda).run();
    }
}
