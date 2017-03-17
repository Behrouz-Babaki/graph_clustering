#!/usr/bin/env python3
# coding: utf-8

import argparse
from bnc_model import Bnc_Model
from basic_model import Basic_Model

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
    
    parser = argparse.ArgumentParser()
    parser.add_argument('graph_file', help='file containing the edges')
    parser.add_argument('cons_file', help='file containing the constraints')
    parser.add_argument('k', type=int, help='number of clusters')
    parser.add_argument('gamma', type=float, help='balance coefficient')
    parser.add_argument('method', choices=['basic', 'bnc'], 
                        help='mip model to be used')
    parser.add_argument('--verbose', action='store_true', 
                        help='print detailed output')
    args = parser.parse_args()    

    n_vertices, edges, constraints, node_ids = read_graph(args.graph_file, 
                                                          args.cons_file)
    verbosity = 1 if args.verbose else 0                                
    k = args.k
    gamma = args.gamma
    
    if args.method == 'bnc':
        m = Bnc_Model(n_vertices, edges, 
                              constraints=constraints, 
                              k=k, gamma=gamma, verbosity=verbosity, 
                              symmetry_breaking=1)
    elif args.method == 'basic':
        m = Basic_Model(n_vertices, edges, 
                              constraints=constraints, 
                              k=k, gamma=gamma, verbosity=verbosity, 
                              symmetry_breaking=1)
    m.solve()
    print(m.clusters)
