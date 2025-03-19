#!/bin/bash
#SBATCH --array=0-23

params="--numKeys 100M --numThreads 1 --numQueries 100M"
loadfactor090="$params --minimalOnly --loadFactor 0.9"
loadfactor095="$params --minimalOnly --loadFactor 0.95"
loadfactor097="$params --minimalOnly --loadFactor 0.97"
jobs=(
    "$loadfactor090 --sichash"
    "$loadfactor090 --pthash"
    "$loadfactor090 --chd"
    "$loadfactor090 --partitionedPthash"
    "$loadfactor095 --sichash"
    "$loadfactor095 --pthash"
    "$loadfactor095 --chd"
    "$loadfactor095 --partitionedPthash"
    "$loadfactor097 --sichash"
    "$loadfactor097 --pthash"
    "$loadfactor097 --chd"
    "$loadfactor097 --partitionedPthash"
    "$params --bipartiteShockHashFlat"
    "$params --densePartitionedPtHash"
    "$params --fiPS"
    "$params --recsplit"
    "$params --simdrecsplit"
    "$params --bbhash"
    "$params --rustFmph"
    "$params --rustFmphGo"
    "$params --shockhash"
    "$params --bipartiteShockHash"
    "$params --consensus"
    "$params --fchPtHash"
    "$params --rustPtrHash"
    "$params --bdz"
    "$params --morphisHash"
    "$params --morphisHashFlat"
)

if [ "$#" -ne 0 ]; then
    # Use "--list" or "--jobs" or actually anything
    # Print available jobs
    for i in "${!jobs[@]}"; do 
        echo -e "$i \t ${jobs[$i]}"
    done
    exit 0
fi

echo "Host: $(hostname)"
echo "Git commit: $(git rev-parse HEAD)"
git status
echo "Gcc version: $(gcc --version)"
echo "-----"

make Comparison
strings Comparison | grep fPIC

if [[ -z "$SLURM_ARRAY_TASK_COUNT" ]]; then
    echo "This is a simple batch execution. Running all tasks."
    for i in "${!jobs[@]}"; do 
        echo "./Comparison ${jobs[$i]}"
        ./Comparison ${jobs[$i]}
    done
else
    echo "This is a slurm array job. Running only the requested task."
    echo "./Comparison ${jobs[$SLURM_ARRAY_TASK_ID]}"
    sleep 5
    ./Comparison ${jobs[$SLURM_ARRAY_TASK_ID]}
fi

