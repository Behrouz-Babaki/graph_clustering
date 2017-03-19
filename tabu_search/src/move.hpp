#pragma once

#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <set>
#include <iostream>

using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::priority_queue;
using std::set;
using std::cout;
using std::endl;

class Move {
public:
    Move(int n_vertices, int n_clusters, double gamma, 
         const vector< vector< int > >& graph,
         const vector< pair< int, int> >& constraints,
         const vector< double >& weights,
         const vector<  int  >& initial_clusters) :
        _n_vertices(n_vertices), 
        _n_clusters(n_clusters), 
        _gamma(gamma), _graph(graph),
        clusters(initial_clusters) {

	cl_pairs.assign(_n_vertices, set< int >());
	cl_weights.assign(_n_vertices, map< int, double >());
        for (int i=0, s=constraints.size(); i<s; i++) {
            int first = constraints[i].first,
                second = constraints[i].second;
            double weight = weights[i];
	    cl_pairs[first].insert(second);
	    cl_pairs[second].insert(first);
	    cl_weights[first][second] = weight;
	    cl_weights[second][first] = weight;
        }
        
        penalties.assign(_n_vertices, vector< double >(_n_clusters, 0));
        for (int i=0; i<_n_vertices; i++) 
	  for (auto u : cl_pairs[i]) 
	    penalties[i][clusters[u]] += cl_weights[i][u];

        cluster_sizes.assign(_n_clusters, 0);
        for (int i=0; i<_n_vertices; i++) 
            cluster_sizes[clusters[i]]++;

        minsize = cluster_sizes[0];
	second_minsize = cluster_sizes[1];
        for (int i=1; i<_n_clusters; i++)
            if(cluster_sizes[i] < minsize) {
		second_minsize = minsize;
                minsize = cluster_sizes[i];
            }

        // initialize valid_moves
        valid_moves.assign(_n_vertices, vector< bool >(_n_clusters, false));
	for (int i=0; i < _n_vertices; i++) {
	  for (auto u : _graph[i])
	    if (clusters[u] != clusters[i])
	      valid_moves[i][clusters[u]] = true;
	}
	
	best_sol = clusters;
	double penalty_sum = 0;
	for(int i=0; i<_n_vertices; i++) 
	  for(auto u : cl_pairs[i])
	    if (clusters[u] == clusters[i])
	      penalty_sum += cl_weights[i][u];
	penalty_sum /= 2;
	best_cost = minsize + gamma * penalty_sum;
	
	
	do_move();
    }
    
    vector< int > get_clusters(){
      return clusters;
    }

private:
  
  void do_move() {
    for (int i=0; i<10; i++) {
    calculate_gains();
    pair< double, pair< int, int > > x = gains.top();
    double improvement = x.first;
    int node = x.second.first, target = x.second.second;
    cout << node << " " << target << " " << improvement << endl;
    if (cluster_sizes[clusters[node]] > 1)
      update(node, target);
    }
  }
  
  void update(int node, int target){
    int origin = clusters[node];
    // update minsize, minsize_cluster, second_minsize
    if (minsize == cluster_sizes[origin]) {
      second_minsize = minsize;
      minsize--;
    }
    else if (minsize == cluster_sizes[target]) {
      if(second_minsize == minsize) {
	minsize = second_minsize;
	second_minsize++;
      }
      else
	minsize++;
    }
    
    // update clusters
    clusters[node] = target;
    
    // update valid_moves
    valid_moves[node][origin] = true;
    valid_moves[node][target] = false;
    
    for (auto u : _graph[node]) {
      valid_moves[node][origin] = true;
      bool to_origin = false;
      for (int i=0, s=_graph[u].size(); !to_origin && i<s; i++)
	if(clusters[_graph[u][i]] == origin)
	  to_origin = true;
      valid_moves[u][origin] = to_origin;
    }

    
    // update penalties
    for(int i=0; i<_n_vertices; i++) {
      if (cl_pairs[i].find(node) == cl_pairs[i].end())
	continue;
      double weight = cl_weights[i][node];
      penalties[i][origin] -= weight;
      penalties[i][target] += weight;
    }
       
  }
  
  void calculate_gains(){
    gains = priority_queue< pair< double, pair< int, int > > >();

    for (int node=0; node < _n_vertices; node++)
      for (int target=0; target < _n_clusters; target++) {
	if(!valid_moves[node][target])
	  continue;
	int origin = clusters[node];
	double improvement = _gamma * (penalties[node][origin] - penalties[node][target]);
	if(cluster_sizes[origin] == minsize)
	  improvement -= 1;
	else if(cluster_sizes[target] == minsize &&
		second_minsize > minsize)
	  improvement += 1;
	
	gains.push(make_pair(improvement, make_pair(node, target)));
      }
  }

    int _n_vertices;
    int _n_clusters;
    double _gamma;
    const vector< vector< int > >& _graph;
    vector< int > cluster_sizes;
    int minsize;
    int second_minsize;
    vector< int > clusters;
    double best_cost;
    vector< int > best_sol;
    vector< set< int > > cl_pairs;
    vector< map< int, double > > cl_weights;
    vector< vector< double > > penalties;
    vector< vector< bool > > valid_moves;
    priority_queue< pair< double, pair< int, int > > > gains;

};
