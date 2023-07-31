#include <tlx/cmdline_parser.hpp>
#include "benchmark/SicHashContender.h"
#include "benchmark/PTHashContender.h"
#include "benchmark/RustFmphGoContender.h"
#include "benchmark/RecSplitContender.h"
#include "benchmark/SIMDRecSplitContender.h"
#include "benchmark/BBHashContender.h"
#include "benchmark/ShockHashContender.h"

int main(int argc, char** argv) {
    size_t N = 5e6;
    tlx::CmdlineParser cmd;
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('q', "numQueries", Contender::numQueries, "Number of queries to perform");
    cmd.add_flag('T', "skipTests", Contender::skipTests, "Skip testing PHF for validity");

    if (!cmd.process(argc, argv)) {
        return 1;
    }

    {SIMDRecSplitContender<5>(N, 5).run();}
    {SIMDRecSplitContender<8>(N, 100).run();}
    {SIMDRecSplitContender<14>(N, 2000).run();}
    {RecSplitContender<5>(N, 5).run();}
    {RecSplitContender<8>(N, 100).run();}
    {RecSplitContender<14>(N, 2000).run();}
    {BBHashContender(N, 1.0, 0).run();}
    {BBHashContender(N, 5.0, 0).run();}
    {RustFmphGoContender(N, 1.0).run();}
    {RustFmphGoContender(N, 5.0).run();}
    {PTHashContender<true, pthash::compact_compact>(N, 0.99, 7.0).run();}
    {PTHashContender<true, pthash::dictionary_dictionary >(N, 0.88, 11.0).run();}
    {PTHashContender<true, pthash::elias_fano >(N, 0.99, 6.0).run();}
    {SicHashContender<true, 64>(N, 0.97, sichash::SicHashConfig().percentages(0.45, 0.31)).run();}
    {SicHashContender<true, 64>(N, 0.9, sichash::SicHashConfig().percentages(0.21, 0.78)).run();}
    {ShockHashContender<30>(N, 100).run();}
    {ShockHashContender<30>(N, 2000).run();}
    {ShockHashContender<39>(N, 2000).run();}
    {ShockHashContender<58>(N, 2000).run();}
    return 0;
}
