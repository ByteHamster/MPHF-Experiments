#!/bin/bash

# Run benchmark
cd /opt/mphf_experiments/build
./Comparison --numKeys 500k --numQueries 0 --loadFactor 0.85 --sichash --sichashOnlyPartial --pthash --chd | tee /opt/dockerVolume/pareto-zoomed.txt
./Comparison --numKeys 500k --numQueries 0 --loadFactor 0.95 --sichash --sichashOnlyPartial --pthash --chd | tee --append /opt/dockerVolume/pareto-zoomed.txt
./Comparison --numKeys 500k --numQueries 0 --recsplit --bbhash --simdrecsplit --shockhash --rustFmph --rustFmphGo | tee --append /opt/dockerVolume/pareto-zoomed.txt

# Build plot
cd /opt/dockerVolume
/opt/sqlplot-tools/build/src/sqlplot-tools pareto-zoomed.tex
rm -f pareto-zoomed.pdf
pdflatex pareto-zoomed.tex
pdflatex pareto-zoomed.tex
rm -f *.out *.log *.aux

