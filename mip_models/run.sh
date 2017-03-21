#!/bin/bash

# echo "Using the basic method with symmetry-breaking:"
# ./runner.py ../data/graph.csv ../data/graph_cannot_links.csv 3 5 basic --verbose

# echo "Using the basic method with overlap:"
# ./runner.py ../data/graph.csv ../data/graph_cannot_links.csv 3 5 basic --verbose --overlap

# echo "Using the branch-and-cut method without symmetry-breaking:"
# ./runner.py ../data/graph.csv ../data/graph_cannot_links.csv 3 5 bnc --verbose --nosym

echo "Using the branch-and-cut method with overlap:"
./runner.py ../data/graph.csv ../data/graph_cannot_links.csv 3 5 bnc --verbose --overlap

