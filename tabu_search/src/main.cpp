#define N_BFS 5

#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <random>
#include <utility>
#include <map>

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
using std::priority_queue;
using std::map;

int bfs(const vector< vector< int > >&,
        const vector< int >& ,
        vector< bool >&, int, int);

pair< int, int > furthest_points(const vector< vector< int > >&,
		     const vector< int >&, vector< bool >&,
		     int, int);

void bisect( const vector< vector< int > >&, 
		vector< int >&,
		vector< bool >&,
		int, int, int, int,
		int&, int&);

void partition(const vector< vector< int > >&, 
	       vector< int >&, 
	       vector< bool >&, int);

void partition(const vector< vector< int > >& graph, 
	       vector< int >& clusters, 
	       vector< bool >& visited, 
	       int num_clusters) {
// partition the graph into num_clusters connected clusters
  int n = graph.size();
  clusters.assign(n, -1);
  priority_queue< pair< int, int >  > sizes;
  sizes.push(make_pair(n, -1));
  int j = 0;
  for (int i=1; i<num_clusters; i++) {
    int current_size, current_id;    
    pair< int, int> cs = sizes.top();
    current_size = cs.first;
    current_id = cs.second;
    sizes.pop();

    int left_size, right_size;
    bisect(graph, clusters, visited, current_id, current_size, 
	   j, j+1, left_size, right_size);
    cout << "partitioned " << current_size << " into " << left_size << " and " << right_size << endl;
    sizes.push(make_pair(left_size, j));
    sizes.push(make_pair(right_size, j+1));
    j += 2;
  }
  
  //canonical numbering
  map< int, int > labels;
  for (int i=0; i<n ;i++) {
    int c = clusters[i];
    if (labels.find(c) == labels.end()) {
      int s = labels.size();
      labels[c] = s;
    }
    clusters[i] = labels[c];
  }
}

void bisect( const vector< vector< int > >& graph, 
		vector< int >& clusters,
		vector< bool >& visited,
		int current_id,
		int current_size,
		int left_id, 
		int right_id,
		int& left_size,
		int& right_size) {
  
  pair< int, int > uv;
  uv = furthest_points(graph, clusters, visited, current_id, current_size);
  int u = uv.first, v = uv.second;
  cout << "u :" << u << " v:" << v << endl;
  int n = graph.size();
  
  visited.assign(n, false);
  visited[u] = true;
  visited[v] = true;
  
  clusters[u] = left_id;
  clusters[v] = right_id;
  
  vector< int > uv_cluster(2);
  uv_cluster[0] = left_id;
  uv_cluster[1] = right_id;

  vector< int > uv_size(2, 1);

  vector< queue< int > > uv_q(2, queue< int >());
  uv_q[0].push(u);
  uv_q[1].push(v);
  
  vector< int > uv_edges(2);
  vector< int > uv_edge_id(2, -1);
 
  int j = 0;
  for(int i=0; i<current_size; i++) {
    bool found = false;
    while (!found && !uv_q[j].empty()) {
      // if index is -1, pop another node, update the uv_edges and index
      if (uv_edge_id[j] == -1) {
	uv_edges[j] = uv_q[j].front();
	uv_q[j].pop();
	uv_edge_id[j] = 0;
      }
      
      // iterate through the list until you find something or reach end
      int k = uv_edge_id[j];
      for (int s=graph[uv_edges[j]].size(); !found && k<s; k++) {
	int node = graph[uv_edges[j]][k];
      // if found, update flag, size, clusters, visited
	if (clusters[node] == current_id && !visited[node]) {
	  found = true;
	  visited[node] = true;
	  clusters[node] = uv_cluster[j];
	  uv_size[j]++;
	  uv_q[j].push(node);
	  uv_edge_id[j] = k<s-1 ? k+1 : -1;
	}
      }
      // if you get to the end, set index to -1
      if (!found)
	uv_edge_id[j] = -1;
    }

    // change the turn only if the other one is not done
    if (!uv_q[1-j].empty())
      j = 1-j;
  }
  
  left_size = uv_size[0];
  right_size = uv_size[1];
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
    partition(g, clusters, visited, nclusters);
    
    for (auto x: clusters)
      cout << x << " ";
    cout << endl;
    
    for (int i=0; i<nclusters; i++)
      cout << check_connected(g, clusters, visited, i) << endl;
    
  return 0;
}
