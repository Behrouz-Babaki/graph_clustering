#pragma once

#include <vector>
#include <utility>

using std::vector;
using std::pair;

class InitialPartition {

public:
    InitialPartition(const vector< vector< int > >& graph, int n_clusters) :
        _graph(graph) , _n_clusters(n_clusters) {
        _n_vertices = _graph.size();
        _clusters.reserve(_n_vertices);
        _visited.reserve(_n_vertices);
	partition();
    }
    
    vector< int > get_clusters();


private:

    const vector < vector< int > >& _graph;
    int _n_vertices;
    int _n_clusters;
    vector< int > _clusters;
    vector< bool > _visited;

    int bfs(int, int);

    pair< int, int > furthest_points(int, int);

    void bisect( int, int, int, int,
                 int&, int&);

    void partition();
};
