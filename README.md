# Top K Shortest Path Problem with MPI and OpenMP

This project tackles the K Shortest Path Problem using a combination of MPI for distributed computing and OpenMP for shared memory parallelization within MPI processes. The goal is to find the Kth shortest path among all nodes of a given graph.

## Problem Description

- **Kth Shortest Path Problem**: Unlike the standard shortest path problem, which aims to find the shortest path, the Kth shortest path problem seeks the Kth shortest path, which may not necessarily be unique.
- **Input**: Read the graph representation from a file. The graph can be a weighted graph. Perform any necessary pre-processing to adapt it according to the requirements of the code given in the link.
- **Initialization**: Initialize a distance matrix with the weights of the graph edges. Set the diagonal elements to zero (distance of a vertex to itself) and infinity for unreachable vertices.
- **Parallelization Strategy**: Divide the computation of the Kth shortest path among MPI processes. Each MPI process will explore a subset of paths. Parallelize the loops within each MPI process using OpenMP.

## Testing with Provided Graphs

Utilize the following graphs:
1. Doctor Who
2. Enron Email
3. EU Email

Execute the given code to compute the Top K shortest path between 10 randomly selected pairs of nodes and show the results. Then execute the MPI+OpenMP code for the same 10 pairs of nodes and compare the results.

## Reporting Requirements

- Discuss challenges faced during preprocessing, implementation, and testing.
- Describe any optimizations applied to improve performance.
- Report the results of experiments for each provided graph, including execution time and achieved speedup with different configurations.

## How to compile
mpicxx -o proj  proj.cpp -std=c++17

## How to execute
mpiexec -n 2 ./proj 
