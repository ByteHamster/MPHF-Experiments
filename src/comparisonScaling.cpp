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
        {PTHashContender<true, pthash::elias_fano>(N, 0.95, 4.0).run();}
        {PartitionedPTHashContender<true, pthash::elias_fano>(N, 0.95, 4.0).run();}
        {PhobicContender<pthash::dense_interleaved<pthash::rice>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 6.5).run();}
        {SicHashContender<true, 64>(N, 0.95, sichash::SicHashConfig().percentages(0.37, 0.44)).run();}
        Contender::numQueries = numQueries / 3;
        {SIMDRecSplitContender<14>(N, 2000).run();}
        {RustFmphGoContender(N, 1.5).run();}
        {RustFmphContender(N, 1.5).run();}
        {BBHashContender(N, 1.5, 0).run();}
        {BipartiteShockHashContender<64>(N, 2000).run();}
        if (Contender::numThreads == 1) {
            {RecSplitContender<14>(N, 2000).run();}
            {ShockHashContender<40>(N, 2000).run();}
            {BipartiteShockHashFlatContender<64>(N).run();}
            {ChdContender(N, 0.95, 0.95, 5, false).run();}
            {BdzContender(N, 0.8, 1.0/0.8, 3, true).run();}
            {FiPSContender<>(N, 1.5).run();}
        }
    }
    return 0;
}
