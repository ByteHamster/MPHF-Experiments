#include "RecSplitContender.h"

template <int l>
void recSplitTestMulti(size_t N) {
    {RecSplitContender<l>(N, l).run();}
    {RecSplitContender<l>(N, 50).run();}
    {RecSplitContender<l>(N, 100).run();}
    {RecSplitContender<l>(N, 500).run();}
    {RecSplitContender<l>(N, 2000).run();}
}

void recSplitContenderRunner(size_t N) {
    recSplitTestMulti<4>(N);
    recSplitTestMulti<5>(N);
    recSplitTestMulti<6>(N);
    recSplitTestMulti<7>(N);
    recSplitTestMulti<8>(N);
    recSplitTestMulti<11>(N);
    recSplitTestMulti<12>(N);
    recSplitTestMulti<13>(N);
    recSplitTestMulti<14>(N);
}
