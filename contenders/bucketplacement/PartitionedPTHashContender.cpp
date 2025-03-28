#include "PartitionedPTHashContender.h"

void partitionedPtHashContenderRunner(size_t N, double loadFactor, bool minimalOnly) {
    for (double lambda = 3.0; lambda < 10.0; lambda += 0.4) {
        if (!minimalOnly) {
            PartitionedPTHashContender<false, pthash::elias_fano>(N, loadFactor, lambda).run();
            PartitionedPTHashContender<false, pthash::dictionary_dictionary>(N, loadFactor, lambda).run();
            PartitionedPTHashContender<false, pthash::dictionary_elias_fano>(N, loadFactor, lambda).run();
            PartitionedPTHashContender<false, pthash::compact_compact>(N, loadFactor, lambda).run();
        }
        PartitionedPTHashContender<true, pthash::elias_fano>(N, loadFactor, lambda).run();
        PartitionedPTHashContender<true, pthash::dictionary_dictionary>(N, loadFactor, lambda).run();
        PartitionedPTHashContender<true, pthash::dictionary_elias_fano>(N, loadFactor, lambda).run();
        PartitionedPTHashContender<true, pthash::compact_compact>(N, loadFactor, lambda).run();
    }
}
