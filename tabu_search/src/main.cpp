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
    if (cl_set.find(p) == cl_set.end() &&
      cl_set.find(make_pair(second, first)) == cl_set.end()) {
      cl_set.insert(p);
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

double evaluate(const vector< int >& clusters, int nclusters,
		const vector< pair< int, int > >& cl,
		const vector< double >& weights, double gamma) {
  double penalty_sum = 0.0;
  for(int i=0; i<(int)cl.size(); i++) {
    int d1 = cl[i].first, d2 = cl[i].second;
    int c1 = clusters[d1], c2 = clusters[d2];
    if (c1 == c2) 
      penalty_sum += weights[i];
  }
  vector< int > sizes(nclusters, 0);
  for (auto c : clusters)
    sizes[c]++;
  int m = sizes[0];
  for (auto nc : sizes)
    if (nc < m)
      m = nc;
  return m - gamma * penalty_sum;
}

int main(int argc, char** argv) {

//     GraphReader gr(argv[1]);
//     vector< vector< int > > g = gr.get_graph();
//     vector< pair< int, int > > constraints;
//     vector< double > weights;

//     double gamma = 0.1;
//     int nclusters = 6;
//    generate_constraints(g.size(), g.size()/2, constraints, weights);
  
      int nclusters = 3;
      double gamma = 5.0;
      GraphReader gr("data/toy.graph");
      vector< vector< int > > g = gr.get_graph();
      vector< pair< int, int > > constraints;
      vector< double > weights;
      gr.get_constraints("data/toy.constraints", constraints, weights);
      
    InitialPartition p(g, nclusters);
    vector< int > clusters = p.get_clusters();
    check_clusters(nclusters, clusters, g);
    cout << evaluate(clusters, nclusters, constraints, weights, gamma) << endl;
   
    Move m (g.size(), nclusters, gamma, g, constraints, weights, clusters); 
    clusters = m.get_clusters();
    check_clusters(nclusters, clusters, g);
    cout << evaluate(clusters, nclusters, constraints, weights, gamma) << endl;


      

    
  return 0;
}
