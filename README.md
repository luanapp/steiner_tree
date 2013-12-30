Steiner Tree
============

This is a genetic algorithm implementation for the steiner tree problem. It follows a problem proposal in the file descricao.pdf (in Portuguese).

The goal isn't to return the best solution for the problem, since it's NP-Complete. This code implements an aproximation solution using a genetic algorithm and the its details will be given bellow (as soon as each part is implemented ;-) ).

Implementation roadmap
----------------------
- [x] Read the file and allocate the adjacency matrix based on the retrieved data.
- [x] Create the root species for any solution for the problem, which will be a Minimum Spanning Tree(MST) containing all terminals.
- [x] Perform mutations on the initial population.
- [ ] Implement a crossover between solutions.
- [ ] Define the _survival of the fittest_ criteria.
- [ ] Create a new population based on the fittest criteria.

Input File Format
-----------------
The first two lines defines how many nodes and edges does the graph contains. Each line has the format: [Graph Part] [Number], where "Graph Part" is the word "Edges" or "Nodes", followed by its quantity separated by a space. The following illustrates the first two lines:

```
Nodes 100
Edges 4950
```

The next _edges_ lines (in the example, 4950) have the information about the graph edges: nodes composing this edge and the edge weight. The lines are like the following example:

```
E 1 2 70
E 1 3 36
E 1 4 467
E 1 5 995
E 1 6 98
E 1 7 559

```
After the last edge there is an empty line, indicating the end of the edges.
The next information is about the nodes that are terminals and, therefore, are mandatory to be included in the solution. The first line indicates the number of terminals in the solution, preceeded by the word "Terminals". The next _terminals_ lines indicates which vertexes are included in the terminal set. The expected format is like the example bellow:

```
Terminals 10
T 57
T 26
T 100
T 88
T 60
T 17
T 27
T 94
T 58
T 74
```





References:
http://amadeus.ecs.umass.edu/mie373/smtg_genetic.pdf
