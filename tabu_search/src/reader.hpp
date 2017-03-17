#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;

class GraphReader{

public:
  GraphReader(string filename);
  vector< vector< int > > get_graph();
  
private:
  vector< vector< int > > graph;
  int n_vertices;
  int n_edges;
};

