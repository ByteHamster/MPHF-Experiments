#include <tlx/cmdline_parser.hpp>

#include "bucketplacement/DensePartitionedPTHashContender.h"
#include "bucketplacement/PartitionedPTHashContender.h"
#include "fingerprinting/FiPSContender.h"
#include "fingerprinting/RustFmphContender.h"
#include "retrievalbased/BdzContender.h"
#include "bucketplacement/PTHashContender.h"
#include "bucketplacement/RustPtrHashContender.h"
#include "fingerprinting/BBHashContender.h"
#include "fingerprinting/RustFmphGoContender.h"
#include "recsplit/RecSplitContender.h"
#include "recsplit/SIMDRecSplitContender.h"
#include "retrievalbased/SicHashContender.h"
#include "shockhash/BipartiteShockHashContender.h"
#include "shockhash/BipartiteShockHashFlatContender.h"
#include "shockhash/ShockHashSimdContender.h"
#include "bucketplacement/ChdContender.h"
#include "consensus/ConsensusContender.h"
#include "bucketplacement/FchContender.h"
#include "shockhash/MorphisHashContender.h"
#include "shockhash/MorphisHashFlatContender.h"

/**
 * Comparison table used in "Modern Minimal Perfect Hashing".
 */
int main(int argc, char** argv) {
    size_t N = 5e6;
    tlx::CmdlineParser cmd;
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('q', "numQueries", Contender::numQueries, "Number of queries to perform");
    cmd.add_bytes('t', "numThreads", Contender::numThreads, "Number of threads to use for construction");
    cmd.add_bytes('t', "numQueryThreads", Contender::numQueryThreads, "Number of threads to use for queries");
    cmd.add_flag('T', "skipTests", Contender::skipTests, "Skip testing PHF for validity");

    if (!cmd.process(argc, argv)) {
        return 1;
    }

    {SicHashContender<true, 64>(N, 0.95, sichash::SicHashConfig().percentages(0.37, 0.44)).run();}
    {SicHashContender<true, 64>(N, 0.97, sichash::SicHashConfig().percentages(0.45, 0.31)).run();}

    {SIMDRecSplitContender<8>(N, 100).run();}
    {SIMDRecSplitContender<16>(N, 2000).run();}

    {ShockHashSimdContender<55>(N, 2000).run();}
    {ShockHashSimdContender<40>(N, 2000).run();}

    {BipartiteShockHashContender<128>(N, 2000).run();}
    {BipartiteShockHashContender<64>(N, 2000).run();}

    {BipartiteShockHashFlatContender<64>(N).run();}
    {BipartiteShockHashFlatContender<100>(N).run();}

    {RustFmphContender(N, 1.5).run();}
    {RustFmphContender(N, 3.5).run();}

    {RustFmphGoContender(N, 1.5).run();}
    {RustFmphGoContender(N, 3.5).run();}

    {BBHashContender(N, 1.5, 0).run();}
    {BBHashContender(N, 3.5, 0).run();}

    {FiPSContender<>(N, 1.5).run();}
    {FiPSContender<>(N, 3.5).run();}

    {ChdContender(N, 1.0, 1.0, 4, true).run();}
    {ChdContender(N, 1.0, 1.0, 6, true).run();}

    {PTHashContender<true, pthash::elias_fano>(N, 0.95, 4.0).run();}
    {PTHashContender<true, pthash::compact_compact>(N, 0.99, 6.0).run();}

    {PartitionedPTHashContender<true, pthash::elias_fano>(N, 0.95, 4.0).run();}
    {PartitionedPTHashContender<true, pthash::compact_compact>(N, 0.99, 6.0).run();}

    {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::compact>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 4.0).run();}
    {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::rice>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 6.0).run();}

    {RecSplitContender<8>(N, 100).run();}
    {RecSplitContender<14>(N, 2000).run();}

    {ConsensusContender<512, 0.1>(N).run();}
    {ConsensusContender<32768, 0.0005>(N).run();}

    {RustPtrHashContender(N, RustPtrHashContender::VARIANT_LINEAR_VEC, 3.0).run();}
    {RustPtrHashContender(N, RustPtrHashContender::VARIANT_CUBIC_EF, 4.0).run();}

    {FchContender(N, 3).run();}
    {FchContender(N, 4).run();}

    {BdzContender(N, 0.8, 1.0/0.8, 3, true).run();}
    {BdzContender(N, 0.6, 1.0/0.8, 6, true).run();}

    {MorphisHashContender<40,4>(N, 2000).run();}
    {MorphisHashContender<64,4>(N, 2000).run();}

    {MorphisHashFlatContender<60,3,2>(N).run();}
    {MorphisHashFlatContender<84,3,2>(N).run();}

    // PHast
    return 0;
}
