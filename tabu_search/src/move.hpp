#pragma once

#include <vector>
#include <map>
#include <utility>
#include <queue>
#include <set>
#include <iostream>
#include <algorithm>

using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::priority_queue;
using std::set;
using std::cout;
using std::endl;
using std::min;

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
	
	//ATTENTION the index orders for valid_moves and articulation_points are different
	articulation_points.assign(_n_clusters, vector< bool >(_n_vertices, false));
	for (int i=0; i<_n_clusters; i++)
	  update_ap(i);
	
	
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
    int node = x.second.first, target = x.second.second;
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
      if(second_minsize > minsize)
	minsize++;
      else {
	        minsize = cluster_sizes[0];
	second_minsize = cluster_sizes[1];
        for (int i=1; i<_n_clusters; i++)
            if(cluster_sizes[i] < minsize) {
		second_minsize = minsize;
                minsize = cluster_sizes[i];
            }
      }
    }
    
    // update clusters
    clusters[node] = target;
    
    // update valid_moves
    valid_moves[node][origin] = true;
    valid_moves[node][target] = false;
    
    for (auto u : _graph[node]) {
      if(clusters[u] != target)
	valid_moves[u][target] = true;
      bool to_origin = false;
      for (int i=0, s=_graph[u].size(); !to_origin && i<s; i++)
	if(clusters[_graph[u][i]] == origin)
	  to_origin = true;
      valid_moves[u][origin] = to_origin;
    }

    
    // update penalties
    for (auto u : cl_pairs[node]) {
      double weight = cl_weights[u][node];
      penalties[u][origin] -= weight;
      penalties[u][target] += weight;
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
  
  void update_ap(int cluster) {
    dfsNumberCounter = 0;
    dfs_num.assign(_n_vertices, -1);
    dfs_low.assign(_n_vertices, 0);
    dfs_parent.assign(_n_vertices, 0);
    articulation_points[cluster].assign(_n_vertices, false);
    for (int i=0; i<_n_vertices; i++) {
      if(clusters[i] != cluster)
	continue;
      if(dfs_num[i] == -1) {
	dfsRoot = i;
	rootChildren = 0;
	ap(i, cluster);
	articulation_points[cluster][dfsRoot] = (rootChildren > 1);
      }
    }
  }
  
  void ap(int u, int cluster) {
    dfs_low[u] = dfs_num[u] = dfsNumberCounter++;
    for (auto v : _graph[u]) {
      if(clusters[v] != cluster)
	continue;
      if (dfs_num[v] == -1) {
	dfs_parent[v] = u;
	if (u == dfsRoot)
	  rootChildren++;
	ap(v, cluster);
	if(dfs_low[v] >= dfs_num[u])
	  articulation_points[cluster][u] = true;
	dfs_low[u] = min(dfs_low[u], dfs_low[v]);
      }
      else if (v != dfs_parent[u])
	dfs_low[u] = min(dfs_low[u], dfs_num[v]);
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
    
    // for finding articulation points
    vector< vector< bool > > articulation_points;
    int dfsNumberCounter;
    int dfsRoot;
    int rootChildren;
    vector< int > dfs_num;
    vector< int > dfs_low;
    vector< int > dfs_parent;

};
