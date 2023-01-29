#include "path.h"
#include "stack.h"
#include "vertices.h"

#include <stdint.h>
#include <stdlib.h>

//The following struct was taken from the assignment doc
//The struct initializes the ADT Graph, which has an amount
//of vertices, could be undirected, and has an array that
//keeps track of whether a city (vertex) has been visited
//or not. Additionally, it has the matrix of positions.

struct Graph {
    uint32_t vertices;
    bool undirected;
    bool visited[VERTICES];
    uint32_t matrix[VERTICES][VERTICES];
};

//The following function was taken from the assignment doc
//This function takes in as input the amount of vertices
//to create the graph with, and a boolean value to
//indicate whether or not the graph shall be undirected.
//It returns a graph.

Graph *graph_create(uint32_t vertices, bool undirected) {
    Graph *G = (Graph *) calloc(1, sizeof(Graph));
    G->vertices = vertices;
    G->undirected = undirected;
    return G;
}

//The following function was taken from the assignment doc
//The function deletes the graph it takes as input and
//frees the memory that was used to hold it. Returns
//nothing.

void graph_delete(Graph **G) {
    free(*G);
    *G = NULL;
    return;
}

//This function returns the number
//of vertices the inputted graph has.

uint32_t graph_vertices(Graph *G) {
    return G->vertices;
}

//This function adds an edge weight between two vertices
//of the graph, the two vertices it takes as parameters and
//the graph itself is also taken as a parameter. It returns
//a boolean to indicate whether this was successful or not.

bool graph_add_edge(Graph *G, uint32_t i, uint32_t j, uint32_t k) {
    if (i < G->vertices && j < G->vertices && i >= 0 && j >= 0) {
        G->matrix[i][j] = k;
        if (G->undirected) {
            G->matrix[j][i] = k;
        }
        return true;
    }
    return false;
}

//This function does almost the same thing as the previous
//function but instead of changing the weight it simply
//checks whether the weight exists. Same inputs and returns.

bool graph_has_edge(Graph *G, uint32_t i, uint32_t j) {
    if (i >= 0 && i < G->vertices && j >= 0 && j < G->vertices && G->matrix[i][j] > 0) {
        return true;
    }
    return false;
}

//This function is almost an exact copy of the previous one,
//yet instead it returns the weight between the vertices.

uint32_t graph_edge_weight(Graph *G, uint32_t i, uint32_t j) {
    if (i >= 0 && i < G->vertices && j >= 0 && j < G->vertices) {
        return G->matrix[i][j];
    }
    return 0;
}

//This function takes in a graph and a vertex,
//and returns a value based on whether the
//vertex has been visited.

bool graph_visited(Graph *G, uint32_t v) {
    if (G->vertices > v) {
        return G->visited[v];
    }
    return false;
}

//This function takes in the same as above but
//marks the vertex as visited, returns nothing.

void graph_mark_visited(Graph *G, uint32_t v) {
    G->visited[v] = true;
}

//This function does the opposite of the above,
//marking the graph as unvisited.

void graph_mark_unvisited(Graph *G, uint32_t v) {
    G->visited[v] = false;
}

//This function prints the contents of the graph,
//taking in only the graph as input and returning
//nothing.

void graph_print(Graph *G) {
    for (uint32_t i = 0; i < G->vertices; ++i) {
        for (uint32_t j = 0; j < G->vertices; ++j) {
            printf("Vertices: i-%d j-%d\n", i, j);
            printf("Value: %u\n", G->matrix[i][j]);
        }
    }
}
