#include <iostream>
#include <vector>
#include <queue>

#include "reader.hpp"
#include "init.hpp"
#include "move.hpp"

using std::cout;
using std::endl;
using std::vector;
using std::queue;

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

int main(int argc, char** argv) {

    GraphReader gr(argv[1]);
    vector< vector< int > > g = gr.get_graph();
    int nclusters = 6;
    InitialPartition p(g, nclusters);
    vector< int > clusters = p.get_clusters();
    
    for (auto x: clusters)
      cout << x << " ";
    cout << endl;
    
    for (int i=0; i<nclusters; i++)
      cout << check_connected(g, clusters, i) << endl;
    
  return 0;
}
