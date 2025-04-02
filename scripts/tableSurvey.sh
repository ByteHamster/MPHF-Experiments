#!/bin/bash

echo "Host: $(hostname)"
echo "Git commit: $(git rev-parse HEAD)"
git status
echo "Gcc version: $(gcc --version)"
echo "-----"

make TableSurvey
strings TableSurvey | grep fPIC

./TableSurvey --numKeys 100M --numThreads 1 --numQueries 100M

