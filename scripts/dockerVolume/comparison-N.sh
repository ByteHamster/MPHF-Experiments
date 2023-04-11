#!/bin/bash

# Run benchmark
./ComparisonN --pthashParameter 3.35 --numKeys 1M    --numQueries 20M --iterations 2 | tee /opt/dockerVolume/comparison-N.txt
./ComparisonN --pthashParameter 3.75 --numKeys 3M    --numQueries 20M --iterations 2 | tee --append /opt/dockerVolume/comparison-N.txt
./ComparisonN --pthashParameter 3.95 --numKeys 10M   --numQueries 20M --iterations 2 | tee --append /opt/dockerVolume/comparison-N.txt
./ComparisonN --pthashParameter 4.30 --numKeys 30M   --numQueries 20M --iterations 2 | tee --append /opt/dockerVolume/comparison-N.txt
./ComparisonN --pthashParameter 4.45 --numKeys 100M  --numQueries 20M --iterations 2 | tee --append /opt/dockerVolume/comparison-N.txt

# Build plot
cd /opt/dockerVolume
/opt/sqlplot-tools/build/src/sqlplot-tools comparison-N.tex
rm -f comparison-N.pdf
pdflatex comparison-N.tex
pdflatex comparison-N.tex
rm -f *.out *.log *.aux

