#pragma once

#include <deque>
#include <stack>
#include <vector>

using std::deque;
using std::stack;
using std::vector;

class BF_Checker {
  
public: 
  BF_Checker(vector< vector < int > > graph, 
	     vector< vector< int > > constraints, 
	     int k, double gamma): _graph(graph), 
	     _constraints(constraints), _gamma(gamma) {
	       
	       this->_feasible = false;
	       this->_first = true;
	       int n = graph.size();
	       this->bf(n, k);
  }
  
  bool is_feasible(void) {
    return this->_feasible;
  }
  
  double get_best(void) {
    return this->_best;
  }
  
private:
    
  vector< vector< int > > _graph;
  vector< vector< int > > _constraints;
  vector< bool > _visited;
  double _gamma;
  bool _feasible;
  double _best;
  bool _first;
  
  void bf(int n, int k) {
    deque<int> s;
    vector< vector< bool > > clusters(k, vector< bool >(n));
    vector< int > sizes(k, 0);

    do {

      while(s.size() < n)
	s.push_back(0);
      
      // s is the current solution
      sizes.assign(k, 0);
      for(int i=0; i<k; i++)
	for (int j=0; j<n; j++)
	  if(s[j] == i) {
	    clusters[i][j] = true;
	    sizes[i]++;
	  }
	  else 
	    clusters[i][j] = false;
	  
      int min_size = sizes[0];
      for (int i=1; i<k; i++)
	if (sizes[i] < min_size)
	  min_size = sizes[i];
	
      bool all_connected = true;
      for (int i=0; all_connected && i<k; i++)
	all_connected &= is_connected(clusters[i]);
      
      if(all_connected) {
	double value = min_size + _gamma * get_penalty(s);
	if (_first) {
	  _best = value;
	  _feasible = true;
	  _first = false;
	}
	else if (value > _best)
	  _best = value;
      }
      
      int t = s.back();
      s.pop_back();
      while (!s.empty() && t == k-1) {
	t = s.back();
	s.pop_back();
      }
      
      if (t<k-1)
	s.push_back(t + 1);
    } while (!s.empty());
  }

  bool is_connected(const vector< bool >& cluster) {
    int n = cluster.size();
    _visited.assign(n, false);
    
    stack<int> s;
    for(int i=0; s.empty() && i<n; i++)
      if (cluster[i])
	s.push(i);
    
    while(!s.empty()){
      int u = s.top();
      s.pop();
      _visited[u] = true;
      for (auto v : _graph[u])
	if (cluster[v] and !_visited[v])
	  s.push(v);
    }
    
    bool connected = true;
    for(int i=0; connected && i<n; i++)
      if(cluster[i] && !_visited[i])
	connected = false;
      
    return connected;
  }
  
  double get_penalty(const deque<int>& clusters) {
    double penalty = 0;
    for (auto constraint : _constraints) {
      int first = constraint[0];
      int second = constraint[1];
      double weight = constraint[2];
      if (clusters[first] == clusters[second])
	penalty += weight;
    }
    return penalty;
  }

};