#!/bin/bash

# Run benchmark
cd /opt/mphf_experiments/build
params="--numKeys 1M --numThreads 1 --numQueries 1M"
loadfactor090="$params --minimalOnly --loadFactor 0.9"
loadfactor095="$params --minimalOnly --loadFactor 0.95"
loadfactor097="$params --minimalOnly --loadFactor 0.97"
tee="tee --append /opt/dockerVolume/dominance-map.txt"
echo "" > /opt/dockerVolume/dominance-map.txt
./Comparison $loadfactor090 --sichash | $tee
./Comparison $loadfactor090 --pthash | $tee
./Comparison $loadfactor090 --chd | $tee
./Comparison $loadfactor090 --partitionedPthash | $tee
./Comparison $loadfactor095 --sichash | $tee
./Comparison $loadfactor095 --pthash | $tee
./Comparison $loadfactor095 --chd | $tee
./Comparison $loadfactor095 --partitionedPthash | $tee
./Comparison $loadfactor097 --sichash | $tee
./Comparison $loadfactor097 --pthash | $tee
./Comparison $loadfactor097 --chd | $tee
./Comparison $loadfactor097 --partitionedPthash | $tee
./Comparison $params --rustSrs | $tee
./Comparison $params --bipartiteShockHashFlat | $tee
./Comparison $params --densePartitionedPtHash | $tee
./Comparison $params --fiPS | $tee
./Comparison $params --recsplit | $tee
./Comparison $params --simdrecsplit | $tee
./Comparison $params --bbhash | $tee
./Comparison $params --rustFmph | $tee
./Comparison $params --rustFmphGo | $tee
./Comparison $params --shockhash | $tee
./Comparison $params --bipartiteShockHash | $tee
./Comparison $params --consensus | $tee
./Comparison $params --fchPtHash | $tee

# Build plot
cd /opt/dockerVolume
/opt/sqlplot-tools/build/src/sqlplot-tools dominance-map.tex
rm -f dominance-map.pdf
pdflatex dominance-map.tex
pdflatex dominance-map.tex
rm -f *.out *.log *.aux

