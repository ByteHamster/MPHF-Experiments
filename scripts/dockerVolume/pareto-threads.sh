#!/bin/bash

# Run benchmark
cd /opt/mphf_experiments/build

params="--numKeys 1M --numThreads 1 --numQueries 0"
./Comparison $params --minimalOnly --loadFactor 0.9   --sichash --pthash --chd --partitionedPthash | tee /opt/dockerVolume/pareto-threads.txt
./Comparison $params --minimalOnly --loadFactor 0.95  --sichash --pthash --chd --partitionedPthash | tee --append /opt/dockerVolume/pareto-threads.txt
./Comparison $params --minimalOnly --loadFactor 0.97  --sichash --pthash --chd --partitionedPthash | tee --append /opt/dockerVolume/pareto-threads.txt
./Comparison $params --recsplit --simdrecsplit --bbhash | tee --append /opt/dockerVolume/pareto-threads.txt

params="--numKeys 16M --numThreads 16 --numQueries 0"
./Comparison $params --minimalOnly --loadFactor 0.9  --pthash --partitionedPthash | tee --append /opt/dockerVolume/pareto-threads.txt
./Comparison $params --minimalOnly --loadFactor 0.95 --pthash --partitionedPthash | tee --append /opt/dockerVolume/pareto-threads.txt
./Comparison $params --minimalOnly --loadFactor 0.97 --pthash --partitionedPthash | tee --append /opt/dockerVolume/pareto-threads.txt
./Comparison $params --simdrecsplit --bbhash | tee --append /opt/dockerVolume/pareto-threads.txt

# Build plot
cd /opt/dockerVolume
/opt/sqlplot-tools/build/src/sqlplot-tools pareto-threads.tex
rm -f pareto-threads.pdf
pdflatex pareto-threads.tex
pdflatex pareto-threads.tex
rm -f *.out *.log *.aux

