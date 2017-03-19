#pragma once

#include <vector>
#include <map>
#include <utility>
#include <queue>

using std::vector;
using std::map;
using std::pair;
using std::make_pair;
using std::priority_queue;

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

	node_to_constraints.assign(_n_vertices, vector< pair< int, double > >());
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
	second_minsize = cluster_sizes[1];
        minsize_cluster = 0;
        for (int i=1; i<_n_clusters; i++)
            if(cluster_sizes[i] < minsize) {
		second_minsize = minsize;
                minsize = cluster_sizes[i];
                minsize_cluster = i;
            }

        // initialize valid_moves
        valid_moves.assign(_n_vertices, vector< bool >(_n_clusters, false));
	for (int i=0; i < _n_vertices; i++) {
	  // extra constraint: no empty clusters!
	  if (cluster_sizes[clusters[i]] == 1)
	    continue;
	  for (auto u : _graph[i])
	    if (clusters[u] != clusters[i])
	      valid_moves[i][clusters[u]] = true;
	}
    }

private:
  
  // TODO update the values of data structures after a move  
  void update(int node, int cluster){
    // update clusters
    // update minsize, minsize_cluster, second_minsize
    // update penalties
    // update valid_moves
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
    int minsize_cluster;
    vector< int > clusters;
    vector< vector< pair< int, double > > > node_to_constraints;
    vector< vector< double > > penalties;
    vector< vector< bool > > valid_moves;
    priority_queue< pair< double, pair< int, int > > > gains;

};
