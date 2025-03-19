#!/bin/bash
hostname
strings ComparisonScaling | grep fPIC

./ComparisonScaling --numKeys 1M    --iterations 4
./ComparisonScaling --numKeys 2M    --iterations 3
./ComparisonScaling --numKeys 5M    --iterations 3
./ComparisonScaling --numKeys 10M   --iterations 2
./ComparisonScaling --numKeys 20M   --iterations 2
./ComparisonScaling --numKeys 50M
./ComparisonScaling --numKeys 100M
./ComparisonScaling --numKeys 200M
