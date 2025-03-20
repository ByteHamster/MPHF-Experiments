#include "RecSplitRotateContender.h"

template <int l>
void recSplitRotateTestMulti(size_t N) {
    {RecSplitRotateContender<l>(N, l).run();}
    {RecSplitRotateContender<l>(N, 50).run();}
    {RecSplitRotateContender<l>(N, 100).run();}
    {RecSplitRotateContender<l>(N, 500).run();}
    {RecSplitRotateContender<l>(N, 2000).run();}
}

void recSplitRotateContenderRunner(size_t N) {
    recSplitRotateTestMulti<4>(N);
    recSplitRotateTestMulti<5>(N);
    recSplitRotateTestMulti<6>(N);
    recSplitRotateTestMulti<7>(N);
    recSplitRotateTestMulti<8>(N);
    recSplitRotateTestMulti<11>(N);
    recSplitRotateTestMulti<12>(N);
    recSplitRotateTestMulti<13>(N);
    recSplitRotateTestMulti<14>(N);
}
