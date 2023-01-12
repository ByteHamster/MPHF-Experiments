#!/bin/bash
hostname
strings ComparisonThreads | grep fPIC

for i in $(seq 1 1 16); do
  threads=$i
  if [[ $threads == 0 ]]; then
    threads=1
  fi
  ./ComparisonThreads --numKeys 50M --numThreads "$threads" --iterations 3 --numQueries 0
done
