#include "PhobicContender.h"

void PhobicContenderRunner(size_t N) {
    for (double lambda = 2.0; lambda < 10.0; lambda += 0.5) {
        { PhobicContender<pthash::dense_dual<pthash::inter_C, pthash::inter_R,   0, 100>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, lambda).run(); }
        { PhobicContender<pthash::dense_dual<pthash::inter_C, pthash::inter_R,  20, 100>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, lambda).run(); }
        { PhobicContender<pthash::dense_dual<pthash::inter_C, pthash::inter_R,  40, 100>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, lambda).run(); }
        { PhobicContender<pthash::dense_dual<pthash::inter_C, pthash::inter_R,  60, 100>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, lambda).run(); }
        { PhobicContender<pthash::dense_dual<pthash::inter_C, pthash::inter_R,  80, 100>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, lambda).run(); }
        { PhobicContender<pthash::dense_dual<pthash::inter_C, pthash::inter_R, 100, 100>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, lambda).run(); }
    }
}
