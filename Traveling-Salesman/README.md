# Assignment 4: The Perambulations of Denver Long

## Program Description:

The purpose of this program is to solve the Traveling Salesman Problem (TSP) using
depth-first search (DFS). First, a graph (matrix, created using an array of arrays)
is created with a certain amount of vertices, defined by the user (this number must 
be less than the predetermined maximum). Each vertex will have a name, generally the 
name of a city. The user also has the option of making the graph undirected using a 
command line argument. There will then be two paths that are created, with one being 
the current path that is being traveled and the other existing to hold the shortest 
path that has been found so far. DFS will then be used, started at the origin of the 
graph, to find the paths. Once the shortest path is found the length of it is printed 
as well as the path itself. Additionally, the number of calls to the DFS function will 
be printed for research purposes. The user has the option to print out all of the paths 
found as well using a command line option.

## How to Build The Program:

Run the command "make" which will use rules provided in the Makefile to build
the code using clang. You can also use "make clean" to remove trash files and
"make format" to format the code to a specific standard defined in the Makefile.

## How to Run The Program:

Type ./tsp -(argument(s)) while in the folder containing the 'tsp' executable, as well
as any other necessary files such as stack.c, stack.h, graph.c, graph.h, path.c, path.h,
vertices.h, and optionally a folder of graphs.

The following command-line arguments are available:
 -i infile: Specifies the input file path for the vertices, cities, and edges. Defaults to stdin.
 -o outfile: Specifies the output file path for the results. Defaults to stdout.
 -v : When enabled, every complete Hamiltonian path that is found is printed.
 -h : Prints out a help page showing these options
 -u : Specifies that a graph be undirected.
