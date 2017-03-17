#define N_BFS 5

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <random>
#include <utility>

#include "reader.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::queue;
using std::random_device;
using std::default_random_engine;
using std::uniform_int_distribution;
using std::pair;
using std::make_pair;

int bfs(const vector< vector< int > >&,
        const vector< int >& ,
        vector< bool >&, int, int);

pair< int, int > furthest_points(const vector< vector< int > >&,
		     const vector< int >&, vector< bool >&,
		     int, int);

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
  cout << "running bisect " << cid_first << " " << cid_last << endl;
  pair< int, int > uv;
  uv = furthest_points(graph, clusters, visited, current_cluster, current_size);
  int u = uv.first, v = uv.second;
  cout << "u :" << u << " v:" << v << endl;
  int n = graph.size();
  
  uv_visited[0].assign(n, false);
  uv_visited[1].assign(n, false);
  vector< int > uv_first(2), uv_last(2);
  uv_first[0] = cid_first;
  uv_last[0] = ceil((cid_first + cid_last) / 2.0);
  uv_first[1] = uv_last[0];
  uv_last[1] = cid_last;
  
  // label the clusters by their first index
  vector< int > uv_cluster(2);
  uv_cluster[0] = uv_first[0];
  uv_cluster[1] = uv_first[1];

  vector< int > uv_size(2, 1);
  vector< queue< int > > uv_q(2, queue< int >());
  
  uv_visited[0][u] = true;
  uv_q[0].push(u);
  
  uv_visited[1][v] = true;
  uv_q[1].push(v);
 
  int j = 0;
  int node;

  for(int i=0; i<current_size; i++) {
    //cout << "i: " << i << endl;
    bool found = false;
      while (!found) {
	node = uv_q[j].front();
	//cout << "node:" << node << endl;
	//cout << clusters[node] << " " << current_cluster << endl;
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
  
  for (int i=0; i<2; i++)
    if (uv_last[i] > uv_first[i] + 1) 
      bisect(graph, clusters, uv_visited, visited, uv_cluster[i],uv_size[i], uv_first[i], uv_last[i]);
}

pair< int, int > furthest_points(const vector< vector< int > >& graph, 
		     const vector< int >& clusters, 
		     vector< bool >& visited,
		     int cluster, int cluster_size) {
  cout << "running furthest_points " << cluster << endl;
  cout << "cluster size:" << cluster_size << endl;

  // This allocation is going to be costly
  vector<int> members(cluster_size);
  int i=0;
  for (int j=0, n=clusters.size(); j<n; j++) 
    if (clusters[j] == cluster) {
      assert(i<cluster_size);
      members[i++] = j;
    }
 
  // randomly select a node
  random_device rd;
  default_random_engine generator(rd());
  uniform_int_distribution< int > ud(0, cluster_size-1);
  int index = ud(generator);
  cout << "index: " << index << endl;
  int first, second = members[index];
  cout << "second: " << second << endl;
  for (int i=0; i<N_BFS; i++) {
    first = second;
    second = bfs(graph, clusters, visited, cluster, first);
    //cout << "f:" << first << "s:" << second << endl;
  }
  return make_pair(first, second);
}


int bfs(const vector< vector< int > >& graph,
        const vector< int >& clusters,
        vector< bool >& visited, int cluster, int source) {
    //cout << "running bfs for cluster " << cluster << " source " << source << endl;
    assert(clusters[source] == cluster);
    int n = graph.size();
    int u = -1;
    visited.assign(n, false);
    queue< int > q;
    q.push(source);

    while (!q.empty()) {
        u = q.front();
        q.pop();
        for (auto v : graph[u]){
            if (!visited[v] && clusters[v] == cluster) {
                visited[v] = true;
                q.push(v);
            }
	}
    }
    return u;
}

bool check_connected(const vector< vector< int > >& graph, 
		     const vector< int >& clusters,
		     vector< bool >& visited, 
		     int cluster) {
  int n = graph.size();
  visited.assign(n, false);
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

int main(int argc, char** argv) {

    GraphReader gr(argv[1]);
    vector< vector< int > > g = gr.get_graph();
    int n = g.size();
    vector< int > clusters(n, 0);
    vector< bool > visited(n, false);
    vector< vector< bool > > uv_visited(2, vector< bool >(n, false));

    
    int nclusters = 6;
    bisect(g, clusters, uv_visited, visited, 0, n, 1, nclusters+1);
    vector<int> cl(nclusters, 0);
    for (auto c : clusters)
     cl[c-1]++;
    for (auto x: cl)
      cout << x << " ";
    cout << endl;
    
    for (int i=1; i<nclusters+1; i++)
      cout << check_connected(g, clusters, visited, i) << " ";
    cout << endl;
    return 0;
}
