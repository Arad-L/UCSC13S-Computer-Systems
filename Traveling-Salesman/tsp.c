#include "graph.h"
#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define OPTIONS     "vui:o:h"
#define BUFFER_SIZE 300

static int recursions = 1;
static bool verbose = false;

//This function was inspired by pseudocode from the assignment doc and tutor session pseudo
//This function uses Depth-First-Search to finds multiple paths (and save the shortest)
//through the graph that is given. The function takes in multiple parameters, including
//a Graph G, a vertex (uint32_t) v, two Path's curr and shortest, a character array cities,
//and an outfile to print the results out to. The function is void and returns nothing.

void DFS(Graph *G, uint32_t v, Path *curr, Path *shortest, char *cities[], FILE *outfile) {

    graph_mark_visited(G, v);
    path_push_vertex(curr, v, G);
    for (uint32_t i = 0;
         i < graph_vertices(G)
         && ((path_length(curr) < path_length(shortest)) || path_length(shortest) == 0);
         ++i) {
        if (graph_has_edge(G, v, i)) {
            if (!graph_visited(G, i)) {
                recursions++;
                DFS(G, i, curr, shortest, cities, outfile);
            } else if (path_vertices(curr) == graph_vertices(G) && i == 0) {
                path_push_vertex(curr, i, G);
                if (verbose) {
                    printf("Path length: %d\n", path_length(curr));
                    printf("Path: ");
                    path_print(curr, outfile, cities);
                }
                if ((path_length(curr) < path_length(shortest)) || path_length(shortest) == 0) {
                    path_copy(shortest, curr);
                }
                path_pop_vertex(curr, &v, G);
            }
        }
    }
    path_pop_vertex(curr, &v, G);
    graph_mark_unvisited(G, v);
}

//The following function is used to print out the help screen when
//a user has not entered the correct arguments or has requested for
//the help screen to appear. This function will also terminate the program.
//The function takes no parameters and returns nothing as it exits the code
//no matter what.

int help() {

    printf("SYNOPSIS\n  Traveling Salesman Problem using DFS.\n\nUSAGE\n  ./tsp [-u] [-v] [-h] "
           "[-i infile] [-o outfile]\n\nOPTIONS\n  -u             Use indirected graph.\n  -v      "
           "       Enable verbose printing.\n  -h             Program usage and help.\n  -i infile "
           "     Input containing graph (default: stdin)\n  -o outfile     Output of computed path "
           "(default: stdout)\n");
    exit(0);
}

//The following function is used to check which arguments were given
//to the program, and then to parse them and, using a switch statement,
//implement each argument and it's purpose into the following program.
//The function takes its arguments as parameters, one being the amount of args
//and the other being a character array of the arguments. The function also
//creates graphs, paths, and takes input from a user or a file to create
//the vertices (cities). The function returns zero to indicate success.

int main(int argc, char **argv) {

    uint32_t vertices;
    char input[BUFFER_SIZE];
    bool undirected = false;
    FILE *outfile = stdout;
    FILE *infile = stdin;
    int opt = 0;

    while ((opt = getopt(argc, argv, OPTIONS)) != -1) {
        switch (opt) {
        case 'v': verbose = true; break;
        case 'u': undirected = true; break;
        case 'i':
            infile = fopen(optarg, "r");
            if (infile == NULL) {
                printf("Error: failed to open infile.\n");
            }
            break;
        case 'o': outfile = fopen(optarg, "w"); break;
        case 'h': help();
        }
    }

    fgets(input, BUFFER_SIZE, infile);
    if (!sscanf(input, "%d", &vertices) || feof(infile) || vertices > VERTICES || vertices < 1) {
        printf("Error: malformed number of vertices.\n");
        exit(0);
    }
    char **cities = (char **) calloc(vertices, sizeof(char *));
    for (uint32_t i = 0; i < vertices; ++i) {
        fgets(input, BUFFER_SIZE, infile);
        if (feof(infile)) {
            printf("Error: malformed city name.\n");
            exit(0);
        }
        input[strlen(input) - 1] = '\0';
        cities[i] = strdup(input);
    }

    Graph *G = graph_create(vertices, undirected);

    uint32_t vert_i, vert_j, weight_k;
    while (true) {
        fgets(input, BUFFER_SIZE, infile);
        if (!sscanf(input, "%d %d %d", &vert_i, &vert_j, &weight_k)) {
            printf("Error: malformed edge.\n");
            exit(0);
        }
        graph_add_edge(G, vert_i, vert_j, weight_k);
        if (feof(infile)) {
            break;
        }
    }

    Path *curr = path_create();
    Path *shortest = path_create();

    if (vertices == 1) {
        printf("There's nowhere to go.\n");
        exit(0);
    }
    DFS(G, START_VERTEX, curr, shortest, cities, outfile);
    printf("Path length: %d\n", path_length(shortest));
    printf("Path: ");
    path_print(shortest, outfile, cities);
    printf("Total recursive calls: %d\n", recursions);

    for (uint32_t indx = 0; indx < vertices; ++indx) {
        free(cities[indx]);
    }
    free(cities);
    graph_delete(&G);
    path_delete(&curr);
    path_delete(&shortest);

    return 0;
}
