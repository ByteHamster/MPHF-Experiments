#include <tlx/cmdline_parser.hpp>
#include "recsplit/GpuRecSplitContender.h"

int main(int argc, char** argv) {
    double loadFactor = 0.8;
    size_t N = 5e6;
    bool gpurecsplit = false;


    tlx::CmdlineParser cmd;
    cmd.add_double('l', "loadFactor", loadFactor, "Load Factor");
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('q', "numQueries", Contender::numQueries, "Number of queries to perform");
    cmd.add_bytes('t', "numThreads", Contender::numThreads, "Number of threads to run benchmarks with");
    cmd.add_flag('T', "skipTests", Contender::skipTests, "Skip testing PHF for validity");

    cmd.add_flag("gpurecsplit", gpurecsplit, "Execute GPU RecSplit benchmark");
    if (!cmd.process(argc, argv)) {
        return 1;
    }

    if (gpurecsplit) {
        gpuRecSplitContenderRunner(N);
    }
    return 0;
}
