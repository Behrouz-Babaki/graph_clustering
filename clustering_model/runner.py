#!/usr/bin/env python3
# coding: utf-8

from clustering_model import Clustering_Model

def read_graph(graph_fname, cons_fname=None):
    node_names = dict()
    node_ids = dict()
    edges = set()
    
    def get_id(name):
        if not name in node_names:
            node_id = len(node_names)
            node_names[name] = node_id
            node_ids[node_id] = name 

        return node_names[name]  

    def add_edge(v1, v2):
        first = min(v1, v2)
        second = max(v1, v2)
        edges.add((first, second))


    with open(graph_fname) as f:
        f.readline()
        for line in f:
            line = line.strip()
            if not line: continue
            [n1, n2] = line.strip().split(',')
            [v1, v2] = [get_id(i) for i in (n1, n2)]
            add_edge(v1, v2)

    n_vertices = max(node_ids.keys()) + 1
    
    
    constraints = []
    if cons_fname is not None:
        with open(cons_fname) as f:
            f.readline()
            for line in f:
                line = line.strip()
                if not line: continue
                [n1, n2, w] = line.split(',')
                [v1, v2] = [get_id(i) for i in (n1, n2)]
                w = float(w)
                constraints.append((v1, v2, w))
    
    return n_vertices, list(edges), constraints, node_ids


if __name__ == '__main__':

    n_vertices, edges, constraints, node_ids = read_graph('../data/graph.csv', 
                                                          '../data/graph_cannot_links.csv')
    cm = Clustering_Model(n_vertices, edges, 
                          constraints=constraints, 
                          k=3, gamma=5, verbosity=1, symmetry_breaking=1)
    cm.solve()
    print(cm.clusters)

