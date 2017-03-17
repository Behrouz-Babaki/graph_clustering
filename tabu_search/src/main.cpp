#define N_BFS 5

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <random>

#include "reader.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::default_random_engine;
using std::uniform_int_distribution;

int bfs(const vector< vector< int > >&,
        const vector< int >& ,
        vector< bool >&, int, int);

void furthest_points(const vector< vector< int > >&,
		     const vector< int >&, vector< bool >&,
		     int, int, int&, int&);

void bisect( const vector< vector< int > >&, 
		vector< int >&,
		vector< vector< bool > >&,
		vector< bool >&,
		int, int, int, int, int);

void bisect( const vector< vector< int > >& graph, 
		vector< int >& clusters,
		vector< vector< bool > >& uv_visited,
		vector< bool >& visited,
		int current_cluster,
		int current_size,
		int cid_first, int cid_last) {
  int u, v;
  furthest_points(graph, clusters, visited, current_cluster, current_size, u, v);

  int u_first = cid_first;
  int u_last = (cid_first + cid_last) / 2;
  int v_first = u_last;
  int v_last = cid_last;
  
  // label the clusters by their first index
  vector< int > uv_cluster(2);
  uv_cluster[0] = u_first;
  uv_cluster[1] = v_first;

  vector< int > uv_size(2, 0);
  vector< queue< int > > uv_q(2);
  
  uv_visited[0][u] = true;
  uv_q[0].push(u);
  
  uv_visited[1][v] = true;
  uv_q[1].push(v);
 
  int j = 0;
  int node;
  for(int i=0; i<current_size; i++) {
    bool found = false;
      while (!found) {
	node = uv_q[j].front();
	uv_q[j].pop();
	if (clusters[node] == current_cluster) {
	  clusters[node] = uv_cluster[j];
	  uv_size[j]++;
	  found = true;
	}
	for (auto x : graph[node])
	  if (!uv_visited[j][x]) {
	    uv_visited[j][x] = true;
	    uv_q[j].push(x);
	  }
      }
    j = 1-j;
  }
  
  if (u_last > u_first + 1) 
    bisect(graph, clusters, uv_visited, visited, uv_cluster[0],uv_size[0], u_first, u_last);
  if (v_last > v_first + 1) 
    bisect(graph, clusters, uv_visited, visited, uv_cluster[1],uv_size[1], v_first, v_last);
}

void furthest_points(const vector< vector< int > >& graph, 
		     const vector< int >& clusters, 
		     vector< bool >& visited,
		     int cluster, int cluster_size, 
		     int& u, int&v) {
  // This allocation is going to be costly
  vector<int> members(cluster_size);
  int i=0;
  for (int j=0, n=clusters.size(); j<n; j++) 
    if (clusters[j] == cluster)
      members[i++] = j;
 
  // randomly select a node
  default_random_engine generator;
  uniform_int_distribution< int > ud(0, cluster_size);
  int first, second = members[ud(generator)];
  for (int i=0; i<N_BFS; i++) {
    first = second;
    second = bfs(graph, clusters, visited, cluster, first);
  }
  u = first, v = second;
}


int bfs(const vector< vector< int > >& graph,
        const vector< int >& clusters,
        vector< bool >& visited, int cluster, int source) {
    assert(clusters[source] == cluster);

    int n = graph.size();
    int u = -1;
    visited.assign(false, n);
    queue< int > q;
    q.push(source);
    while (!q.empty()) {
        u = q.front();
        q.pop();
        for (auto v : graph[u])
            if (!visited[v] && clusters[v] == cluster) {
                visited[v] = true;
                q.push(v);
            }
    }
    return u;
}


int main(int argc, char** argv) {

    GraphReader gr(argv[1]);
    vector< vector< int > > g = gr.get_graph();

    for (auto u : g) {
        for (auto v : u)
            cout << v << " ";
        cout << endl;
    }

    return 0;
}
