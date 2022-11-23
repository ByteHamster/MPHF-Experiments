#!/bin/bash
# shellcheck disable=SC2086
hostname
strings Comparison | grep fPIC

params="--numKeys 10M --numThreads 1 --numQueries 0"
./Comparison $params --minimalOnly --loadFactor 0.9   --sichash --pthash --chd --partitionedSichash --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.95  --sichash --pthash --chd --partitionedSichash --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.97  --sichash --pthash --chd --partitionedSichash --partitionedPthash
./Comparison $params --recsplit --simdrecsplit --bbhash

params="--numKeys 40M --numThreads 4 --numQueries 0"
./Comparison $params --minimalOnly --loadFactor 0.9  --pthash --partitionedSichash --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.95 --pthash --partitionedSichash --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.97 --pthash --partitionedSichash --partitionedPthash
./Comparison $params --simdrecsplit --bbhash

params="--numKeys 80M --numThreads 8 --numQueries 0"
./Comparison $params --minimalOnly --loadFactor 0.9  --pthash --partitionedSichash --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.95 --pthash --partitionedSichash --partitionedPthash
./Comparison $params --minimalOnly --loadFactor 0.97 --pthash --partitionedSichash --partitionedPthash
./Comparison $params --simdrecsplit --bbhash
