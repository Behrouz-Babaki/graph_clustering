#pragma once

#include <vector>
#include <map>
#include <utility>

using std::vector;
using std::map;
using std::pair;
using std::make_pair;

class Move {
public:
    Move(int n_vertices, int n_clusters, double gamma, 
         const vector< vector< int > >& graph,
         const vector< pair< int, int> >& constraints,
         const vector< double >& weights,
         const vector<  int  >& initial_clusters) :
        _n_vertices(n_vertices), 
        _n_clusters(n_clusters), 
        _gamma(gamma), clusters(initial_clusters) {

	node_to_constraints.assign(_n_vertices);
        for (int i=0, s=constraints.size(); i<s; i++) {
            int first = constraints[i].first,
                second = constraints[i].second;
            double weight = weights[i];
            node_to_constraints[first].push_back(make_pair(second, weight));
            node_to_constraints[second].push_back(make_pair(first, weight));
        }
        
        penalties.assign(_n_vertices, vector< double >(_n_clusters, 0));
        for (int i=0; i<_n_vertices; i++) 
	  for (auto c : node_to_constraints[i]) {
	    int u = c.first;
	    int w = c.second;
	    penalties[i][clusters[u]] += w;
	  }
	

        cluster_sizes.assign(_n_clusters, 0);
        for (int i=0; i<_n_vertices; i++) 
            cluster_sizes[clusters[i]]++;
        

        minsize = cluster_sizes[0];
        minsize_cluster = 0;
        for (int i=1; i<_n_clusters; i++)
            if(cluster_sizes[i] < minsize) {
                minsize = cluster_sizes[i];
                minsize_cluster = i;
            }

        // initialize valid_moves
        valid_moves.assign(_n_vertices, vector< bool >(_n_clusters, false));
	for (int i=0; i < _n_vertices; i++)
	  for (auto u : graph[i])
	    if (clusters[u] != clusters[i])
	      valid_moves[i][clusters[u]] = true;
    }

private:
  
  // TODO update the values of data structures after a move  
  void update(int node, int cluster){
  }
  
  // TODO calculate gains for valid moves assuming that the data is updated
  void calculate_gains(){
  }

    int _n_vertices;
    int _n_clusters;
    double _gamma;
    vector< int > cluster_sizes;
    int minsize;
    int minsize_cluster;
    vector< int > clusters;
    vector< vector< pair< int, double > > > node_to_constraints;
    vector< vector< double > > penalties;
    vector< vector< bool > > valid_moves;
    const vector< vector< int > >& graph;

};
