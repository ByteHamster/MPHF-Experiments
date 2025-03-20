#include "GpuPhobicContender.h"

void gpuPhobicContenderRunner(size_t N) {
    for (float averageBucketSize = 2.0; averageBucketSize <= 12; averageBucketSize += 0.5) {
        for (float tradeoff = 0; tradeoff <= 1; tradeoff += 0.2) {
            { GpuPhobicContender(N, averageBucketSize, 2048, tradeoff).run(); }
        }
    }
}
