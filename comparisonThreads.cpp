#include <tlx/cmdline_parser.hpp>
#include "benchmark/PTHashContender.h"
#include "benchmark/PartitionedPTHashContender.h"
#include "benchmark/BBHashContender.h"
#include "benchmark/SIMDRecSplitContender.h"
#include "benchmark/RustFmphContender.h"
#include "benchmark/RustFmphGoContender.h"

int main(int argc, char** argv) {
    size_t N = 5e6;
    size_t iterations = 1;
    double pthashParameter = 6.0;
    size_t numQueries = 0;
    tlx::CmdlineParser cmd;
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('i', "iterations", iterations, "Number of iterations to execute");
    cmd.add_double('p', "pthashParameter", pthashParameter, "Parameter of the pthash method");
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
        {PTHashContender<true, pthash::elias_fano>(N, 0.95, pthashParameter).run();}
        {PartitionedPTHashContender<true, pthash::elias_fano>(N, 0.95, pthashParameter).run();}
        {RustFmphContender(N, 2.0).run();}
        {RustFmphGoContender(N, 2.0).run();}
        Contender::numQueries = numQueries / 3;
        {SIMDRecSplitContender<10>(N, 2000).run();}
        {BBHashContender(N, 2.0).run();}
    }
    return 0;
}
