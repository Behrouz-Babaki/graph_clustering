#include <iostream>
#include <vector>
#include <queue>
#include <random>
#include <set>

#include "reader.hpp"
#include "init.hpp"
#include "move.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::random_device;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::uniform_real_distribution;
using std::set;

bool check_connected(const vector< vector< int > >& graph, 
		     const vector< int >& clusters,
		     int cluster) {
  int n = graph.size();
  vector< bool > visited(n, false);
  queue< int > q;
  bool found = false;
  for (int i=0; !found && i<n; i++)
    if (clusters[i] == cluster) {
      q.push(i);
      visited[i] = true;
      found = true;
    }
    
  while(!q.empty()){
    int u = q.front();
    q.pop();
    for (auto v: graph[u])
      if (!visited[v] && clusters[v]==cluster) {
	visited[v] = true;
	q.push(v);
      }
  }
  
  for (int i=0; i<n; i++)
    if (clusters[i] == cluster && !visited[i])
      return false;
    
   return true;
}

void generate_constraints(int n, size_t m, 
			  vector< pair< int, int > >& cl_vec,
			  vector< double >& weights) {
  set< pair< int, int > > cl_set;
  cl_vec.clear();
  weights.clear();
  random_device rd;
  default_random_engine generator(rd());
  uniform_int_distribution< int > uid(0, n-1);
  uniform_real_distribution< double > urd(0.0, 1.0);
  while (cl_vec.size() < m) {
    int first = uid(generator);
    int second = uid(generator);
    if(first == second)
      continue;
    pair< int, int > p = make_pair(first, second);
    if (cl_set.find(p) == cl_set.end()) {
      cl_vec.push_back(p);
      weights.push_back(urd(generator));
    }
  }
}

void check_clusters(int nclusters, 
  const vector< int >& clusters, 
  const vector< vector< int > >& graph) {
    for (auto x: clusters)
      cout << x << " ";
    cout << endl;
    
    for (int i=0; i<nclusters; i++)
      cout << check_connected(graph, clusters, i) << endl;
}

int main(int argc, char** argv) {

    GraphReader gr(argv[1]);
    vector< vector< int > > g = gr.get_graph();
    vector< pair< int, int > > constraints;
    vector< double > weights;
    
    int nclusters = 6;
    InitialPartition p(g, nclusters);
    vector< int > clusters = p.get_clusters();
    check_clusters(nclusters, clusters, g);
    
    generate_constraints(g.size(), g.size()/10, constraints, weights);
    Move m (g.size(), nclusters, 0.1, g, constraints, weights, clusters); 
    clusters = m.get_clusters();
    check_clusters(nclusters, clusters, g);
    

    
  return 0;
}
