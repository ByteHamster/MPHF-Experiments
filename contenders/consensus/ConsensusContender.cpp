#include "ConsensusContender.h"

void consensusContenderRunner(size_t N) {
    ConsensusContender<256, 0.5>(N).run();
    ConsensusContender<256, 0.1>(N).run();
    ConsensusContender<512, 0.1>(N).run();
    ConsensusContender<512, 0.06>(N).run();
    ConsensusContender<512, 0.03>(N).run();
    ConsensusContender<32768, 0.1>(N).run();
    ConsensusContender<32768, 0.06>(N).run();
    //ConsensusContender<32768, 0.03>(N).run();
    ConsensusContender<32768, 0.01>(N).run();
    ConsensusContender<32768, 0.006>(N).run();
    ConsensusContender<32768, 0.003>(N).run();
    ConsensusContender<32768, 0.001>(N).run();
    ConsensusContender<32768, 0.0005>(N).run();
}
