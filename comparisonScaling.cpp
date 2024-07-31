#include <tlx/cmdline_parser.hpp>
#include "benchmark/CmphContender.h"
#include "benchmark/SicHashContender.h"
#include "benchmark/PTHashContender.h"
#include "benchmark/RecSplitContender.h"
#include "benchmark/BBHashContender.h"
#include "benchmark/SIMDRecSplitContender.h"
#include "benchmark/DensePartitionedPTHashContender.h"
#include "benchmark/PartitionedPTHashContender.h"
#include "benchmark/BipartiteShockHashContender.h"
#include "benchmark/RustFmphGoContender.h"
#include "benchmark/ShockHashContender.h"
#include "benchmark/BipartiteShockHashFlatContender.h"
#include "benchmark/FiPSContender.h"

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
        {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::rice>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 6.5).run();}
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
            {CmphContender(N, 0.95, "CHD", CMPH_CHD_PH, 0.95, 5, false).run();}
            {CmphContender(N, 0.8, "BDZ", CMPH_BDZ, 1.0/0.8, 3, true).run();}
            {FiPSContender(N, 1.5).run();}
        }
    }
    return 0;
}
