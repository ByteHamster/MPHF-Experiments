#!/bin/bash

# Run benchmark
cd /opt/mphf_experiments/build
./TableThesisLehmann --numKeys 500k --numQueries 500k | tee /opt/dockerVolume/table-thesis-lehmann.txt

# Build plot
cd /opt/dockerVolume
/opt/sqlplot-tools/build/src/sqlplot-tools pareto-zoomed.tex
rm -f table-thesis-lehmann.pdf
pdflatex table-thesis-lehmann.tex
pdflatex table-thesis-lehmann.tex
rm -f *.out *.log *.aux

