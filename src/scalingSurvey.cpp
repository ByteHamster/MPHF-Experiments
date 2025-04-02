#include <bucketplacement/FchContender.h>
#include <bucketplacement/RustPtrHashContender.h>
#include <consensus/ConsensusContender.h>
#include <shockhash/MorphisHashContender.h>
#include <shockhash/MorphisHashFlatContender.h>
#include <shockhash/ShockHashSimdContender.h>
#include <tlx/cmdline_parser.hpp>

#include "bucketplacement/ChdContender.h"
#include "bucketplacement/PhobicContender.h"
#include "bucketplacement/PartitionedPTHashContender.h"
#include "fingerprinting/FiPSContender.h"
#include "fingerprinting/RustFmphContender.h"
#include "retrievalbased/BdzContender.h"
#include "bucketplacement/PTHashContender.h"
#include "fingerprinting/BBHashContender.h"
#include "fingerprinting/RustFmphGoContender.h"
#include "recsplit/RecSplitContender.h"
#include "recsplit/SIMDRecSplitContender.h"
#include "retrievalbased/SicHashContender.h"
#include "shockhash/BipartiteShockHashContender.h"
#include "shockhash/BipartiteShockHashFlatContender.h"
#include "shockhash/ShockHashContender.h"

/**
 * Comparison table used in "Modern Minimal Perfect Hashing".
 */
int main(int argc, char** argv) {
    size_t N = 5e6;
    size_t iterations = 1;
    size_t numQueries = 15e7;
    tlx::CmdlineParser cmd;
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('i', "iterations", iterations, "Number of iterations to execute");
    cmd.add_bytes('q', "numQueries", numQueries, "Number of queries to perform");
    cmd.add_bytes('t', "numThreads", Contender::numThreads, "Number of threads to run benchmarks with");
    cmd.add_flag('T', "skipTests", Contender::skipTests, "Skip testing PHF for validity");

    if (!cmd.process(argc, argv)) {
        return 1;
    }

    for (size_t i = 0; i < iterations; i++) {
        // Queries of PTHash and SicHash have quite a bit of noise in the measurements.
        // Run more queries to work around that.
        Contender::numQueries = numQueries;
        {PTHashContender<true, pthash::elias_fano>(N, 0.95, 6.0).run();}
        {PartitionedPTHashContender<true, pthash::elias_fano>(N, 0.95, 6.0).run();}
        {PhobicContender<pthash::dense_interleaved<pthash::rice>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 6.0).run();}
        {RustPtrHashContender(N, RustPtrHashContender::VARIANT_LINEAR_VEC, 3.0).run();}
        {SicHashContender<true, 64>(N, 0.95, sichash::SicHashConfig().percentages(0.37, 0.44)).run();}

        Contender::numQueries = numQueries / 3;
        {SIMDRecSplitContender<8>(N, 100).run();}
        {RustFmphContender(N, 5.0).run();}
        {RustFmphGoContender(N, 5.0).run();}
        {BBHashContender(N, 5.0, 0).run();}
        {BipartiteShockHashContender<64>(N, 2000).run();}
        {MorphisHashContender<64,4>(N, 2000).run();}

        // Competitors that are only single-threaded
        if (Contender::numThreads == 1) {
            {RecSplitContender<8>(N, 100).run();}
            {ShockHashSimdContender<40>(N, 2000).run();}
            {BipartiteShockHashFlatContender<64>(N).run();}
            {BdzContender(N, 0.8, 1.0/0.8, 3, true).run();}
            {FiPSContender<256, uint32_t, false>(N, 5.0).run();}
            {MorphisHashFlatContender<84,3,2>(N).run();}
            {FchContender(N, 7).run();}
            {ChdContender(N, 1.0, 1.0, 4, true).run();}
            {ConsensusContender<512, 0.1>(N).run();}
        }
    }
    return 0;
}
