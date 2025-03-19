#include "MorphisHashFlatContender.h"

template <int l, int e>
void morphisHashFlatContenderRunnerMultiWidth(size_t N) {
    {MorphisHashFlatContender<l, e, 1>(N).run();}
    {MorphisHashFlatContender<l, e, 2>(N).run();}
    {MorphisHashFlatContender<l, e, 3>(N).run();}
    {MorphisHashFlatContender<l, e, 4>(N).run();}
    {MorphisHashFlatContender<l, e, 5>(N).run();}
    {MorphisHashFlatContender<l, e, 6>(N).run();}
}

template <int l>
void morphisHashFlatContenderRunnerMultiExtraBit(size_t N) {
    morphisHashFlatContenderRunnerMultiWidth<l, 2>(N);
    morphisHashFlatContenderRunnerMultiWidth<l, 3>(N);
    morphisHashFlatContenderRunnerMultiWidth<l, 4>(N);
    morphisHashFlatContenderRunnerMultiWidth<l, 5>(N);
}

void morphisHashFlatContenderRunner(size_t N) {
    morphisHashFlatContenderRunnerMultiExtraBit<28>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<34>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<40>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<46>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<52>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<58>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<64>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<70>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<76>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<82>(N);
    morphisHashFlatContenderRunnerMultiExtraBit<88>(N);
}
