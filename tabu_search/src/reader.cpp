#include <fstream>
#include <vector>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <utility>
#include "reader.hpp"

using std::ifstream;
using std::vector;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::istringstream;
using std::getline;
using std::pair;
using std::make_pair;

GraphReader::GraphReader(string filename)
{
  ifstream gfile(filename, ifstream::in);
  if(gfile.is_open()) {
    string line;
    getline(gfile, line);
    istringstream iss(line);
    iss >> this->n_vertices >> this->n_edges;

    for(int i = 0; i < n_vertices; i++) {
      getline(gfile, line);
      istringstream iss(line);
      vector<int> nodes;
      int node;
      while (iss >> node) {
	node--;
	nodes.push_back(node);
      }
      this->graph.push_back(nodes);
    }
  }
  else {
    cerr << "error opening graph file" << endl;
    exit(1);
  }
  gfile.close();
}

vector< vector< int > > GraphReader::get_graph()
{
  return this->graph;
}

void GraphReader::get_constraints(string filename, 
				  vector< pair< int, int > >& cl,
				  vector< double >& weights){
  cl.clear();
  weights.clear();
  
  ifstream cfile(filename, ifstream::in);
  if(cfile.is_open()) {
    string line;
    int first, second;
    double weight;
    while(getline(cfile, line)) {
      istringstream iss(line);
      iss >> first >> second >> weight;
      cl.push_back(make_pair(first, second));
      weights.push_back(weight);
    }
  }
  else {
    cerr << "error opening constraint file" << endl;
    exit(1);
  }
  cfile.close();
}
