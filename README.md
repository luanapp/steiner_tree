Steiner Tree
============

This is a genetic algorithm implementation for the steiner tree problem. It follows a problem proposal in the file descricao.pdf (in Portuguese).

The goal isn't to return the best solution for the problem, since it's NP-Complete. This code implements an aproximation solution using a genetic algorithm and the its details will be given bellow (as soon as each part is implemented ;-) ).

Implementation roadmap
----------------------
- [x] Read the file and allocate the adjacency matrix based on the retrieved data.
- [] Create the root species for any solution for the problem, which will be a Minimum Spanning Tree(MST) containing all terminals.
- [] Perform mutations on a given solution/population.
- [] Implement a crossover between solutions.
- [] Define the _survival of the fittest_ criteria.
- [] Create a new population based on the previous criteria.

File Format
-----------





References:
http://amadeus.ecs.umass.edu/mie373/smtg\_genetic.pdf
