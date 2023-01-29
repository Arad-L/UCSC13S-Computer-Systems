#include "path.h"

#include "stack.h"
#include "vertices.h"

#include <stdlib.h>

//The following struct was taken from the assignment doc
//It creates the ADT Path, which carries a stack of
//vertices and a length based on the total weight of the
//path.

struct Path {
    Stack *vertices; // The vertices comprising the path.
    uint32_t length; // The total length of the path.
};

//This function creates a path as empty
//and with no length. It takes in no parameters,
//but returns a path.

Path *path_create(void) {
    Path *p = (Path *) malloc(sizeof(Path));
    p->length = 0;
    p->vertices = stack_create(VERTICES);
    if (!p->vertices) {
        free(p);
        p = NULL;
    }
    return p;
}

//This function deletes the path it takes
//in as a parameter, returning nothing but
//freeing the memory the path was taking.

void path_delete(Path **p) {
    if (*p && (*p)->vertices) {
        stack_delete(&(*p)->vertices);
        free(*p);
        *p = NULL;
    }
    return;
}

//This function returns a boolean value
//indicating whether it successfully
//pushed a vertex onto the stack, using
//the parameters of a graph from which to
//take a vertex, the path onto which it
//will be pushed, and the vertex itself.

bool path_push_vertex(Path *p, uint32_t v, Graph *G) {
    uint32_t prev = 0;
    stack_peek(p->vertices, &prev);
    if (v == prev) {
        return stack_push(p->vertices, v);
    }
    p->length += graph_edge_weight(G, prev, v);
    return stack_push(p->vertices, v);
}

//This function was influenced by tutors.
//This function pops out a vertex, taking
//as parameters the graph to refer to for vertex name,
//the path from which to pop, and a pointer to
//a variable that will hold the popped value.
//The function returns a boolean to indicate
//success.

bool path_pop_vertex(Path *p, uint32_t *v, Graph *G) {
    if (stack_pop(p->vertices, v)) {
        uint32_t prev = 0;
        stack_peek(p->vertices, &prev);
        if (*v == prev) {
            return true;
        } else {
            p->length -= graph_edge_weight(G, prev, *v);
        }
        return true;
    }
    return false;
}

//The following function takes in a path
//as input and returns the size of the
//vertices stack it holds.

uint32_t path_vertices(Path *p) {
    return stack_size(p->vertices);
}

//This function in a path as input and returns
//its length as a uint32_t.

uint32_t path_length(Path *p) {
    return p->length;
}

//This function takes in two paths, one
//is a destination path and one is a source
//path. The source is copied to the destination
//and nothing is returned.

void path_copy(Path *dst, Path *src) {
    if (dst && src) {
        stack_copy(dst->vertices, src->vertices);
        dst->length = src->length;
    }
    return;
}

//This function takes in as input a path, an outfile,
//and the array of cities. It prints out the path.
//It returns nothing.

void path_print(Path *p, FILE *outfile, char *cities[]) {
    stack_print(p->vertices, outfile, cities);
    return;
}
