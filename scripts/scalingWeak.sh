#!/bin/bash
hostname
strings ComparisonN | grep fPIC

for i in $(seq 1 1 8); do
  threads=$i
  if [[ $threads == 0 ]]; then
    threads=1
  fi
  ./ComparisonN --numKeys "$((10*threads))M" --numThreads "$threads" --iterations 2 --numQueries 0 --minimalOnly
done
