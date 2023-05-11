#include <tlx/cmdline_parser.hpp>
#include "benchmark/BBHashContender.h"
#include "benchmark/CmphContender.h"
#include "benchmark/SicHashContender.h"
#include "benchmark/PTHashContender.h"
#include "benchmark/RecSplitContender.h"
#include "benchmark/SIMDRecSplitContender.h"
#include "benchmark/RecSplitRotateContender.h"
#include "benchmark/MphfWbpmContender.h"
#include "benchmark/ShockHashContender.h"
#include "benchmark/PartitionedPTHashContender.h"

int main(int argc, char** argv) {
    double loadFactor = 0.8;
    size_t N = 5e6;
    bool recsplit = false;
    bool simdrecsplit = false;
    bool recsplitRotate = false;
    bool mphfWbpm = false;
    bool bbhash = false;
    bool sichash = false;
    bool pthash = false;
    bool partitionedPthash = false;
    bool shockhash = false;
    bool chd = false;
    bool bdz = false;
    bool bmz = false;
    bool chm = false;
    bool fch = false;
    bool sichashOnlyPartial = false;
    bool minimalOnly = false;

    tlx::CmdlineParser cmd;
    cmd.add_double('l', "loadFactor", loadFactor, "Load Factor");
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('q', "numQueries", Contender::numQueries, "Number of queries to perform");
    cmd.add_bytes('t', "numThreads", Contender::numThreads, "Number of threads to run benchmarks with");
    cmd.add_flag('M', "minimalOnly", minimalOnly, "Skip generation of non-minimal variants in case the method supports both");
    cmd.add_flag('T', "skipTests", Contender::skipTests, "Skip testing PHF for validity");

    cmd.add_flag("recsplit", recsplit, "Execute RecSplit benchmark");
    cmd.add_flag("simdrecsplit", simdrecsplit, "Execute SIMDRecSplit benchmark");
    cmd.add_flag("recSplitRotate", recsplitRotate, "Execute RecSplit benchmark with rotation fitting");
    cmd.add_flag("mphfWbpm", mphfWbpm, "Execute mphfWbpm benchmark");
    cmd.add_flag("bbhash", bbhash, "Execute bbhash benchmark");
    cmd.add_flag("sichash", sichash, "Execute sichash benchmark");
    cmd.add_flag("shockhash", shockhash, "Execute shockhash benchmark");
    cmd.add_flag("sichashOnlyPartial", sichashOnlyPartial, "Ignore fast ribbon retrieval configurations and test fewer thresholds");
    cmd.add_flag("pthash", pthash, "Execute pthash benchmark");
    cmd.add_flag("partitionedPthash", partitionedPthash, "Execute partitioned pthash benchmark");
    cmd.add_flag("chd", chd, "Execute chd benchmark");
    cmd.add_flag("bdz", bdz, "Execute bdz benchmark");
    cmd.add_flag("bmz", bmz, "Execute bmz benchmark");
    cmd.add_flag("chm", chm, "Execute chm benchmark");
    cmd.add_flag("fch", fch, "Execute fch benchmark");

    if (!cmd.process(argc, argv)) {
        return 1;
    }

    if (recsplit) {
        recSplitContenderRunner(N);
    }
    if (simdrecsplit) {
        simdRecSplitContenderRunner(N);
    }
    if (recsplitRotate) {
        recSplitRotateContenderRunner(N);
    }
    if (bbhash) {
        bbHashContenderRunner(N);
    }
    if (chd) {
        chdContenderRunner(N, loadFactor);
    }
    if (bdz) {
        bdzContenderRunner(N, loadFactor);
    }
    if (bmz) {
        bmzContenderRunner(N, loadFactor);
    }
    if (chm) {
        chmContenderRunner(N, loadFactor);
    }
    if (fch) {
        fchContenderRunner(N, loadFactor);
    }
    if (sichash) {
        sicHashContenderRunner<64>(N, loadFactor, minimalOnly);
        if (!sichashOnlyPartial) {
            sicHashContenderRunner<32>(N, loadFactor, minimalOnly);
        }
    }
    if (shockhash) {
        shockHashContenderRunner(N);
    }
    if (pthash) {
        ptHashContenderRunner(N, loadFactor, minimalOnly);
    }
    if (partitionedPthash) {
        partitionedPtHashContenderRunner(N, loadFactor, minimalOnly);
    }
    if (mphfWbpm) {
        mphfWbpmContenderRunner(N);
    }
    return 0;
}
