#include <tlx/cmdline_parser.hpp>

#include "bucketplacement/DensePartitionedPTHashContender.h"
#include "bucketplacement/PartitionedPTHashContender.h"
#include "fingerprinting/FiPSContender.h"
#include "fingerprinting/RustFmphContender.h"
#include "bucketplacement/PTHashContender.h"
#include "recsplit/SIMDRecSplitContender.h"
#include "retrievalbased/SicHashContender.h"
#include "shockhash/BipartiteShockHashFlatContender.h"
#include "bucketplacement/ChdContender.h"
#include "bucketplacement/RustPhastContender.h"
#include "bucketplacement/RustPtrHashContender.h"
#include "consensus/ConsensusContender.h"

/**
* Comparison table used in "PHast".
*/
int main(int argc, char** argv) {
    size_t N = 5e6;
    tlx::CmdlineParser cmd;
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('q', "numQueries", Contender::numQueries, "Number of queries to perform");
    cmd.add_bytes('t', "numThreads", Contender::numThreads, "Number of threads to use for construction");
    cmd.add_flag('T', "skipTests", Contender::skipTests, "Skip testing PHF for validity");

    if (!cmd.process(argc, argv)) {
        return 1;
    }

    {ConsensusContender<256, 0.1>(N).run();}
    {ConsensusContender<512, 0.03>(N).run();}

    {FiPSContender<>(N, 1.5).run();}
    {FiPSContender<>(N, 2.0).run();}

    {RustPhastContender(N, 6, 320).run();}
    {RustPhastContender(N, 7, 370).run();}
    {RustPhastContender(N, 8, 410).run();}
    {RustPhastContender(N, 8, 430).run();}
    {RustPhastContender(N, 8, 450).run();}
    {RustPhastContender(N, 9, 510).run();}
    {RustPhastContender(N, 9, 530).run();}
    {RustPhastContender(N, 10, 620).run();}
    {RustPhastContender(N, 10, 650).run();}

    {RustPtrHashContender(N, RustPtrHashContender::VARIANT_LINEAR_VEC, 3.0).run();}
    {RustPtrHashContender(N, RustPtrHashContender::VARIANT_CUBIC_EF, 4.0).run();}

    {PTHashContender<true, pthash::compact_compact>(N, 0.99, 4.0).run();}
    {PTHashContender<true, pthash::elias_fano>(N, 0.99, 5.0).run();}

    {PartitionedPTHashContender<true, pthash::compact_compact>(N, 0.99, 4.0).run();}
    {PartitionedPTHashContender<true, pthash::elias_fano>(N, 0.99, 5.0).run();}

    {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::compact>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 3.9).run();}
    {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::rice>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 4.5).run();}
    {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::rice>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 6.5).run();}
    {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::compact>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 6.5).run();}
    {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::rice>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 9.0).run();}

    {SicHashContender<true, 64>(N, 0.97, sichash::SicHashConfig().percentages(0.45, 0.31)).run();}
    {SicHashContender<true, 64>(N, 0.9, sichash::SicHashConfig().percentages(0.21, 0.78)).run();}

    {SIMDRecSplitContender<5>(N, 5).run();}
    {SIMDRecSplitContender<8>(N, 100).run();}

    {RustFmphContender(N, 1.0).run();}
    {RustFmphContender(N, 2.0).run();}

    {ChdContender(N, 1.0, 1.0, 3, true).run();}

    {BipartiteShockHashFlatContender<64>(N).run();}

    return 0;
}
