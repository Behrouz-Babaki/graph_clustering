#!/bin/bash

echo "Using the basic method:"
./runner.py ../data/graph.csv ../data/graph_cannot_links.csv 3 5 basic --verbose

echo "Using the branch-and-cut method:"
./runner.py ../data/graph.csv ../data/graph_cannot_links.csv 3 5 bnc --verbose
