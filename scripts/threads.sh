#!/bin/bash
hostname
strings ComparisonN | grep fPIC

for i in $(seq 0 8 64); do
  threads=$i
  if [[ $threads == 0 ]]; then
    threads=1
  fi
  ./ComparisonN --numKeys "$((10*threads))M" --numThreads "$threads" --iterations 2 --minimalOnly
done
