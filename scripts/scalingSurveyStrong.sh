#!/bin/bash

echo "Host: $(hostname)"
echo "Git commit: $(git rev-parse HEAD)"
git status
echo "Gcc version: $(gcc --version)"
echo "-----"

make ScalingSurvey
strings ScalingSurvey | grep fPIC

nproc=$(nproc)
step=$((nproc / 16))
for i in $(seq 0 $step $nproc); do
  threads=$i
  if [[ $threads == 1 ]]; then
    continue; # Otherwise a 16-core machine runs 1 thread twice
  fi
  if [[ $threads == 0 ]]; then
    threads=1
  fi
  ./ScalingSurvey --numKeys 100M --numThreads "$threads" --iterations 1 --numQueries 0
done
