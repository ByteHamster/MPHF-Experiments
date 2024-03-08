#include <tlx/cmdline_parser.hpp>
#include "benchmark/SicHashContender.h"
#include "benchmark/PTHashContender.h"
#include "benchmark/PartitionedPTHashContender.h"
#include "benchmark/RustFmphContender.h"
#include "benchmark/SIMDRecSplitContender.h"
#include "benchmark/BipartiteShockHashContender.h"
#include "benchmark/FchContender.h"
#include "benchmark/GpuPtHashContender.h"

int main(int argc, char** argv) {
    size_t N = 5e6;
    tlx::CmdlineParser cmd;
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('q', "numQueries", Contender::numQueries, "Number of queries to perform");
    cmd.add_flag('T', "skipTests", Contender::skipTests, "Skip testing PHF for validity");

    if (!cmd.process(argc, argv)) {
        return 1;
    }

    {SicHashContender<true, 64>(N, 0.97, sichash::SicHashConfig().percentages(0.45, 0.31)).run();}
    {SicHashContender<true, 64>(N, 0.9, sichash::SicHashConfig().percentages(0.21, 0.78)).run();}

    {SIMDRecSplitContender<5>(N, 5).run();}
    {SIMDRecSplitContender<8>(N, 100).run();}
    {SIMDRecSplitContender<14>(N, 2000).run();}

    {BipartiteShockHashContender<64>(N, 2000).run();}
    {BipartiteShockHashContender<64>(N, 64).run();}

    {RustFmphContender(N, 1.0).run();}
    {RustFmphContender(N, 2.0).run();}

    {FchContender(N, 1.0, 4).run();}
    {FchContender(N, 1.0, 6).run();}

    {PTHashContender<true, pthash::compact_compact>(N, 0.99, 7.0).run();}
    {PTHashContender<true, pthash::dictionary_dictionary>(N, 0.88, 11.0).run();}
    {PTHashContender<true, pthash::elias_fano >(N, 0.99, 6.0).run();}

    {PartitionedPTHashContender<true, pthash::compact_compact>(N, 0.99, 7.0).run();}
    {PartitionedPTHashContender<true, pthash::dictionary_dictionary>(N, 0.88, 11.0).run();}
    {PartitionedPTHashContender<true, pthash::elias_fano >(N, 0.99, 6.0).run();}

    {GpuPtHashContender(N, 5, 2048, 0.0).run();}
    {GpuPtHashContender(N, 7, 2048, 1.0).run();}
    {GpuPtHashContender(N, 7, 2048, 0.0).run();}
    {GpuPtHashContender(N, 10, 2048, 1.0).run();}
    {GpuPtHashContender(N, 12, 2048, 1.0).run();}

    return 0;
}
