#include <iostream>
#include <vector>

#include "bfcheck.hpp"

using std::endl;
using std::cout;
using std::cin;
using std::vector;


int main(void) {
  vector< vector < int > > graph;
  vector< int > node;
    
  // node 0
  node.push_back(1);
  node.push_back(2);
  node.push_back(3);
  graph.push_back(node);
  
  // node 1
  node.clear();
  node.push_back(0);
  graph.push_back(node);
  
  // node 2
  node.clear();
  node.push_back(0);
  node.push_back(3);
  graph.push_back(node);
  
  // node 3
  node.clear();
  node.push_back(2);
  graph.push_back(node);
  
  vector< vector < double > > constraints;
  double gamma = 0.5;
  int k = 2;
  BF_Checker bf(graph, constraints, k, gamma);
  cout << bf.is_feasible() << " " << bf.get_best() << endl;
  return 0;

}

