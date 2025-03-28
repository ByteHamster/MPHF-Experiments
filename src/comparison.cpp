#include <tlx/cmdline_parser.hpp>

#include "fingerprinting/BBHashContender.h"
#include "retrievalbased/SicHashContender.h"
#include "bucketplacement/PTHashContender.h"
#include "recsplit/RecSplitContender.h"
#include "recsplit/RecSplitRotateContender.h"
#include "retrievalbased/MphfWbpmContender.h"
#include "shockhash/ShockHashContender.h"
#include "bucketplacement/PartitionedPTHashContender.h"
#include "fingerprinting/RustFmphContender.h"
#include "fingerprinting/RustFmphGoContender.h"
#include "bucketplacement/RustPhastContender.h"
#include "bucketplacement/RustPtrHashContender.h"
#include "shockhash/BipartiteShockHashContender.h"
#include "bucketplacement/FchContender.h"
#include "bucketplacement/DensePartitionedPTHashContender.h"
#include "consensus/ConsensusContender.h"
#include "shockhash/BipartiteShockHashFlatContender.h"
#include "fingerprinting/FiPSContender.h"
#include "bucketplacement/ChdContender.h"
#include "retrievalbased/BdzContender.h"
#include "retrievalbased/BmzContender.h"
#include "retrievalbased/ChmContender.h"
#include "bucketplacement/FchCmphContender.h"
#include "shockhash/MorphisHashContender.h"
#include "shockhash/MorphisHashFlatContender.h"

#ifdef HAS_VULKAN
#include "bucketplacement/GpuPhobicContender.h"
#endif

#ifdef SIMD
#include "recsplit/SIMDRecSplitContender.h"
#include "shockhash/ShockHashSimdContender.h"
#endif

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
    bool fchCmph = false;
    bool fchPtHash = false;
    bool rustFmphContender = false;
    bool rustFmphGoContender = false;
    bool rustPhastContender = false;
    bool rustPtrHashContender = false;
    bool sichashOnlyPartial = false;
    bool bipartiteShockHash = false;
    bool gpuPhobic = false;
    bool densePartitionedPtHash = false;
    bool bipartiteShockHashFlat = false;
    bool fiPS = false;
    bool consensus = false;
    bool morphisHash = false;
    bool morphisHashFlat = false;
    bool minimalOnly = false;

    tlx::CmdlineParser cmd;
    cmd.add_double('l', "loadFactor", loadFactor, "Load Factor");
    cmd.add_bytes('n', "numKeys", N, "Number of objects");
    cmd.add_bytes('q', "numQueries", Contender::numQueries, "Number of queries to perform");
    cmd.add_bytes('t', "numThreads", Contender::numThreads, "Number of threads to run benchmarks with");
    cmd.add_bytes('t', "numQueryThreads", Contender::numQueryThreads, "Number of threads to run queries with");
    cmd.add_flag('M', "minimalOnly", minimalOnly, "Skip generation of non-minimal variants in case the method supports both");
    cmd.add_flag('T', "skipTests", Contender::skipTests, "Skip testing PHF for validity");
    cmd.add_bytes('s', "seed", Contender::seed, "Seed for pseudo-number generator");

    cmd.add_flag("recsplit", recsplit, "Execute RecSplit benchmark");
    cmd.add_flag("simdrecsplit", simdrecsplit, "Execute SIMDRecSplit benchmark");
    cmd.add_flag("recSplitRotate", recsplitRotate, "Execute RecSplit benchmark with rotation fitting");
    cmd.add_flag("mphfWbpm", mphfWbpm, "Execute mphfWbpm benchmark");
    cmd.add_flag("bbhash", bbhash, "Execute bbhash benchmark");
    cmd.add_flag("sichash", sichash, "Execute sichash benchmark");
    cmd.add_flag("shockhash", shockhash, "Execute shockhash benchmark");
    cmd.add_flag("bipartiteShockHash", bipartiteShockHash, "Execute bipartite shockhash benchmark");
    cmd.add_flag("bipartiteShockHashFlat", bipartiteShockHashFlat, "Execute bipartite shockhash flat benchmark");
    cmd.add_flag("morphisHash", morphisHash, "Execute bipartite morphisHash benchmark");
    cmd.add_flag("morphisHashFlat", morphisHashFlat, "Execute bipartite morphisHash flat benchmark");
    cmd.add_flag("sichashOnlyPartial", sichashOnlyPartial, "Ignore fast ribbon retrieval configurations and test fewer thresholds");
    cmd.add_flag("pthash", pthash, "Execute pthash benchmark");
    cmd.add_flag("partitionedPthash", partitionedPthash, "Execute partitioned pthash benchmark");
    cmd.add_flag("densePartitionedPtHash", densePartitionedPtHash, "Execute dense partitioend PTHash benchmark");
    cmd.add_flag("chd", chd, "Execute chd benchmark");
    cmd.add_flag("bdz", bdz, "Execute bdz benchmark");
    cmd.add_flag("bmz", bmz, "Execute bmz benchmark");
    cmd.add_flag("chm", chm, "Execute chm benchmark");
    cmd.add_flag("fchCmph", fchCmph, "Execute fch (cmph) benchmark");
    cmd.add_flag("fchPtHash", fchPtHash, "Execute fch (PTHash reimplementation) benchmark");
    cmd.add_flag("rustFmph", rustFmphContender, "Execute rust fmph benchmark");
    cmd.add_flag("rustFmphGo", rustFmphGoContender, "Execute rust fmph-go benchmark");
    cmd.add_flag("rustPHast", rustPhastContender, "Execute rust PHast benchmark");
    cmd.add_flag("rustPtrHash", rustPtrHashContender, "Execute rust ptrhash benchmark");
    cmd.add_flag("gpuPhobic", gpuPhobic, "Execute Phobic on the GPU benchmark");
    cmd.add_flag("fiPS", fiPS, "Execute FiPS benchmark");
    cmd.add_flag("consensus", consensus, "Execute Consensus benchmark");

    if (!cmd.process(argc, argv)) {
        return 1;
    }
    if (rustFmphContender) {
        rustFmphContenderRunner(N);
    }
    if (rustFmphGoContender) {
        rustFmphGoContenderRunner(N);
    }
    if (rustPhastContender) {
        rustPHastContenderRunner(N);
    }
    if (rustPtrHashContender) {
        rustPtrHashContenderRunner(N);
    }
    if (recsplit) {
        recSplitContenderRunner(N);
    }
    if (simdrecsplit) {
        #ifdef SIMD
            simdRecSplitContenderRunner(N);
        #else
            std::cerr<<"SIMDRecSplit competitor requested but not compiled into this binary"<<std::endl;
        #endif
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
    if (fchCmph) {
        fchCmphContenderRunner(N, loadFactor);
    }
    if (fchPtHash) {
        fchPtHashContenderRunner(N);
    }
    if (sichash) {
        sicHashContenderRunner(N, loadFactor, minimalOnly, sichashOnlyPartial);
    }
    if (shockhash) {
        #ifdef SIMD
            shockHashSimdContenderRunner(N);
        #else
            shockHashContenderRunner(N);
        #endif
    }
    if (bipartiteShockHash) {
        bipartiteShockHashContenderRunner(N);
    }
    if (bipartiteShockHashFlat) {
        bipartiteShockHashFlatContenderRunner(N);
    }
    if (morphisHash) {
        morphisHashContenderRunner(N);
    }
    if (morphisHashFlat) {
        morphisHashFlatContenderRunner(N);
    }
    if (pthash) {
        ptHashContenderRunner(N, loadFactor, minimalOnly);
    }
    if (partitionedPthash) {
        partitionedPtHashContenderRunner(N, loadFactor, minimalOnly);
    }
    if (densePartitionedPtHash) {
        densePartitionedPtHashContenderRunner(N);
    }
    if (gpuPhobic) {
        #ifdef HAS_VULKAN
            gpuPhobicContenderRunner(N);
        #else
            std::cerr<<"Vulkan competitor requested but not compiled into this binary"<<std::endl;
        #endif
    }
    if (mphfWbpm) {
        mphfWbpmContenderRunner(N);
    }
    if (fiPS) {
        fiPSContenderRunner(N);
    }
    if (consensus) {
        consensusContenderRunner(N);
    }
    return 0;
}
