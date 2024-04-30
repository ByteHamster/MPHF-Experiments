#!/bin/bash
hostname
strings Comparison | grep fPIC

params="--numKeys 100M --numThreads 1 --numQueries 100M"
./Comparison $params --minimalOnly --loadFactor 0.9   --sichash --pthash --chd --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.95  --sichash --pthash --chd --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.97  --partitionedPthash
./Comparison $params --recsplit --simdrecsplit --bbhash --rustFmph --rustFmphGo --densePartitionedPtHash --shockhash --bipartiteShockHash

