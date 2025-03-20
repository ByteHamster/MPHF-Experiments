#include "ShockHashContender.h"

template <int l>
void shockHashTestMulti(size_t N) {
    {ShockHashContender<l>(N, 100).run();}
    {ShockHashContender<l>(N, 500).run();}
    {ShockHashContender<l>(N, 2000).run();}
}

void shockHashContenderRunner(size_t N) {
    shockHashTestMulti<4>(N);
    shockHashTestMulti<6>(N);
    shockHashTestMulti<8>(N);
    shockHashTestMulti<10>(N);
    shockHashTestMulti<12>(N);
    shockHashTestMulti<15>(N);
    shockHashTestMulti<24>(N);
    shockHashTestMulti<28>(N);
    shockHashTestMulti<32>(N);
    shockHashTestMulti<34>(N);
    shockHashTestMulti<36>(N);
    shockHashTestMulti<40>(N);
    shockHashTestMulti<42>(N);
    shockHashTestMulti<44>(N);
    shockHashTestMulti<46>(N);
    shockHashTestMulti<48>(N);
    shockHashTestMulti<50>(N);
    shockHashTestMulti<52>(N);
    shockHashTestMulti<54>(N);
}
