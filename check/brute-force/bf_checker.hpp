#pragma once

#include <deque>
#include <vector>

using std::deque;
using std::vector;

class BF_Checker {
  
public:
  BF_Checker(vector< vector < int > > graph, 
	     vector< vector< int > > constraints, 
	     int k, double gamma);
  bool is_feasible(void);
  double get_best(void);
  
private:
    
  vector< vector< int > > _graph;
  vector< vector< int > > _constraints;
  vector< bool > _visited;
  double _gamma;
  bool _feasible;
  double _best;
  bool _first;
  
  void bf(int n, int k);
  bool is_connected(const vector< bool >& cluster);
  double get_penalty(const deque<int>& clusters);

};