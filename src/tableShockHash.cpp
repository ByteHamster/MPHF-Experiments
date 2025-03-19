#include <tlx/cmdline_parser.hpp>
#include "SicHashContender.h"
#include "PTHashContender.h"
#include "PartitionedPTHashContender.h"
#include "RustFmphContender.h"
#include "SIMDRecSplitContender.h"
#include "BipartiteShockHashContender.h"
#include "DensePartitionedPTHashContender.h"
#include "CmphContender.h"
#include "RustFmphGoContender.h"
#include "BBHashContender.h"
#include "RecSplitContender.h"
#include "ShockHashContender.h"
#include "BipartiteShockHashFlatContender.h"
#include "FiPSContender.h"

/**
 * Comparison table used in "ShockHash: Near Optimal-Space Minimal Perfect Hashing Beyond Brute-Force".
 * https://arxiv.org/abs/2310.14959
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

    {SicHashContender<true, 64>(N, 0.90, sichash::SicHashConfig().percentages(0.21, 0.78)).run();}
    {SicHashContender<true, 64>(N, 0.97, sichash::SicHashConfig().percentages(0.45, 0.31)).run();}

    {SIMDRecSplitContender<8>(N, 100).run();}
    {SIMDRecSplitContender<14>(N, 2000).run();}
    {SIMDRecSplitContender<16>(N, 2000).run();}
    {SIMDRecSplitContender<18>(N, 2000).run();}
    {SIMDRecSplitContender<20>(N, 2000).run();}

    {ShockHashContender<30>(N, 2000).run();}
    {ShockHashContender<39>(N, 2000).run();}
    {ShockHashContender<58>(N, 2000).run();}

    {BipartiteShockHashContender<64>(N, 2000).run();}
    {BipartiteShockHashContender<104>(N, 2000).run();}
    {BipartiteShockHashContender<128>(N, 2000).run();}

    {BipartiteShockHashFlatContender<100>(N).run();}
    {BipartiteShockHashFlatContender<128>(N).run();}

    {RustFmphContender(N, 1.0).run();}
    {RustFmphContender(N, 2.15).run();}

    {RustFmphGoContender(N, 1.0).run();}
    {RustFmphGoContender(N, 2.65).run();}

    {PTHashContender<true, pthash::elias_fano>(N, 0.99, 6.0).run();}
    {PTHashContender<true, pthash::compact_compact>(N, 0.99, 7.0).run();}

    {RecSplitContender<8>(N, 100).run();}
    {RecSplitContender<14>(N, 2000).run();}

    return 0;
}
