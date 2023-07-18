#!/bin/bash
# shellcheck disable=SC2086
hostname
strings Comparison | grep fPIC

params="--numKeys 10M --numThreads 1 --numQueries 0"
./Comparison $params --minimalOnly --loadFactor 0.9   --sichash --pthash --chd --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.95  --sichash --pthash --chd --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.97  --sichash --pthash --chd --partitionedPthash
./Comparison $params --recsplit --simdrecsplit --bbhash --shockhash

params="--numKeys 160M --numThreads 16 --numQueries 0"
./Comparison $params --minimalOnly --loadFactor 0.9  --pthash --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.95 --pthash --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.97 --pthash --partitionedPthash
./Comparison $params --simdrecsplit --bbhash
