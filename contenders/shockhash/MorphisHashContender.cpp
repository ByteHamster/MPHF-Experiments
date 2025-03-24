#include "MorphisHashContender.h"

template <int l, int ws>
void morphisHashTestMultiBucket(size_t N) {
    {MorphisHashContender<l, ws>(N, 100).run();}
    {MorphisHashContender<l, ws>(N, 500).run();}
    {MorphisHashContender<l, ws>(N, 2000).run();}
}

template <int l>
void morphisHashTestMultiWidth(size_t N) {
    morphisHashTestMultiBucket<l, 2>(N);
    morphisHashTestMultiBucket<l, 3>(N);
    morphisHashTestMultiBucket<l, 4>(N);
}

void morphisHashContenderRunner(size_t N) {
    morphisHashTestMultiWidth<28>(N);
    morphisHashTestMultiWidth<34>(N);
    morphisHashTestMultiWidth<40>(N);
    morphisHashTestMultiWidth<46>(N);
    morphisHashTestMultiWidth<52>(N);
    morphisHashTestMultiWidth<58>(N);
    morphisHashTestMultiWidth<64>(N);
    morphisHashTestMultiWidth<70>(N);
    morphisHashTestMultiWidth<76>(N);
    morphisHashTestMultiWidth<82>(N);
    morphisHashTestMultiWidth<88>(N);
    morphisHashTestMultiWidth<94>(N);
}
