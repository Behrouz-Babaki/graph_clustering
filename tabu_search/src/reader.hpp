#pragma once

#include <vector>
#include <string>
#include <utility>

using std::vector;
using std::string;
using std::pair;

class GraphReader{

public:
  GraphReader(string filename);
  vector< vector< int > > get_graph();
  void get_constraints(string, vector< pair< int, int > >&,
		       vector< double >&);  
  
private:
  vector< vector< int > > graph;
  int n_vertices;
  int n_edges;
};

