#!/bin/bash

# Run benchmark
cd /opt/sichash/build
./Comparison --numKeys 5M --numQueries 0 --loadFactor 0.85 --sichash --sichashOnlyPartial --pthash --chd | tee /opt/dockerVolume/pareto.txt
./Comparison --numKeys 5M --numQueries 0 --loadFactor 0.95 --sichash --sichashOnlyPartial --pthash --chd | tee --append /opt/dockerVolume/pareto.txt
./Comparison --numKeys 5M --numQueries 0 --recsplit --bbhash | tee --append /opt/dockerVolume/pareto.txt

# Build plot
cd /opt/dockerVolume
/opt/sqlplot-tools/build/src/sqlplot-tools pareto.tex
rm -f pareto.pdf
pdflatex pareto.tex
pdflatex pareto.tex
rm -f *.out *.log *.aux

