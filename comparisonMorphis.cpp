#include <tlx/cmdline_parser.hpp>
#include "benchmark/SicHashContender.h"
#include "benchmark/PTHashContender.h"
#include "benchmark/PartitionedPTHashContender.h"
#include "benchmark/RustFmphContender.h"
#include "benchmark/SIMDRecSplitContender.h"
#include "benchmark/BipartiteShockHashContender.h"
#include "benchmark/DensePartitionedPTHashContender.h"
#include "benchmark/CmphContender.h"
#include "benchmark/RustFmphGoContender.h"
#include "benchmark/BBHashContender.h"
#include "benchmark/RecSplitContender.h"
#include "benchmark/ShockHashContender.h"
#include "benchmark/BipartiteShockHashFlatContender.h"
#include "benchmark/FiPSContender.h"
#include "benchmark/MorphisHashContender.h"
#include "benchmark/MorphisHashFlatContender.h"
#include "benchmark/ConsensusContender.h"

int main(int argc, char** argv) {
    size_t N = 1e7;
    tlx::CmdlineParser cmd;
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('q', "numQueries", Contender::numQueries, "Number of queries to perform");
    cmd.add_bytes('t', "numThreads", Contender::numThreads, "Number of threads to use for construction");

    if (!cmd.process(argc, argv)) {
        return 1;
    }

    {ConsensusContender<32768, 0.003>(N).run();}


    {BipartiteShockHashContender<56>(N, 2000).run();}

    {MorphisHashContender<46,4>(N, 2000).run();}

    {BipartiteShockHashFlatContender<94>(N).run();}

    {MorphisHashFlatContender<86,3,2>(N).run();}

    {DensePartitionedPTHashContender<pthash::dense_interleaved<pthash::rice>, pthash::table_bucketer<pthash::opt_bucketer>>(N, 1.0, 8.8).run();}
    return 0;
}
