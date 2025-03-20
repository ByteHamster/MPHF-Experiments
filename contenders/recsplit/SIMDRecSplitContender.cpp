#include "SIMDRecSplitContender.h"

template <int l>
void simdRecSplitTestMulti(size_t N) {
    {SIMDRecSplitContender<l>(N, l).run();}
    {SIMDRecSplitContender<l>(N, 50).run();}
    {SIMDRecSplitContender<l>(N, 100).run();}
    {SIMDRecSplitContender<l>(N, 500).run();}
    {SIMDRecSplitContender<l>(N, 2000).run();}
}

void simdRecSplitContenderRunner(size_t N) {
    simdRecSplitTestMulti<4>(N);
    simdRecSplitTestMulti<5>(N);
    simdRecSplitTestMulti<6>(N);
    simdRecSplitTestMulti<7>(N);
    simdRecSplitTestMulti<8>(N);
    simdRecSplitTestMulti<9>(N);
    simdRecSplitTestMulti<10>(N);
    simdRecSplitTestMulti<11>(N);
    simdRecSplitTestMulti<12>(N);
    simdRecSplitTestMulti<13>(N);
    simdRecSplitTestMulti<14>(N);
    simdRecSplitTestMulti<15>(N);
    simdRecSplitTestMulti<16>(N);
}
