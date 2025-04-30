#!/bin/bash

echo "Host: $(hostname)"
echo "Git commit: $(git rev-parse HEAD)"
git status
echo "Gcc version: $(gcc --version)"
echo "-----"

make ScalingSurvey
strings ScalingSurvey | grep fPIC

./ScalingSurvey --numKeys 1M    --iterations 4
./ScalingSurvey --numKeys 2M    --iterations 3
./ScalingSurvey --numKeys 5M    --iterations 3
./ScalingSurvey --numKeys 10M   --iterations 2
./ScalingSurvey --numKeys 20M   --iterations 2
./ScalingSurvey --numKeys 50M
./ScalingSurvey --numKeys 100M
./ScalingSurvey --numKeys 200M
