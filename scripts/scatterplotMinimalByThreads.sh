#!/bin/bash
hostname
strings Comparison | grep fPIC

for threads in 1 4 8; do
  ./Comparison --numKeys "$((10*threads))M" --numThreads "$threads" --numQueries 0 --loadFactor 0.9   --sichash --pthash --chd
  ./Comparison --numKeys "$((10*threads))M" --numThreads "$threads" --numQueries 0 --loadFactor 0.95  --sichash --pthash --chd
  ./Comparison --numKeys "$((10*threads))M" --numThreads "$threads" --numQueries 0 --loadFactor 0.97  --sichash --pthash --chd
  ./Comparison --numKeys "$((10*threads))M" --numThreads "$threads" --numQueries 0 --recsplit --bbhash
done
