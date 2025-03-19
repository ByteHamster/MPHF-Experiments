#include "BipartiteShockHashContender.h"

template <int l>
void bipartiteShockHashTestMulti(size_t N) {
    {BipartiteShockHashContender<l>(N, 100).run();}
    {BipartiteShockHashContender<l>(N, 500).run();}
    {BipartiteShockHashContender<l>(N, 2000).run();}
}

void bipartiteShockHashContenderRunner(size_t N) {
    bipartiteShockHashTestMulti<16>(N);
    bipartiteShockHashTestMulti<24>(N);
    bipartiteShockHashTestMulti<32>(N);
    bipartiteShockHashTestMulti<40>(N);
    bipartiteShockHashTestMulti<48>(N);
    bipartiteShockHashTestMulti<56>(N);
    bipartiteShockHashTestMulti<64>(N);
    bipartiteShockHashTestMulti<72>(N);
    bipartiteShockHashTestMulti<80>(N);
    bipartiteShockHashTestMulti<88>(N);
    bipartiteShockHashTestMulti<96>(N);
    bipartiteShockHashTestMulti<104>(N);
    bipartiteShockHashTestMulti<112>(N);
    bipartiteShockHashTestMulti<120>(N);
    bipartiteShockHashTestMulti<128>(N);
}
