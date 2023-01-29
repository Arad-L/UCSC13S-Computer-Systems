#include "stack.h"

#include <stdlib.h>

//The following struct was taken from the assignment doc
//The stack creates the ADT Stack, which carries an array
//that carries the items within the stack, a variable
//counting its size, and a maximum capacity

struct Stack {
    uint32_t top; // Index of the next empty slot.
    uint32_t capacity; // Number of items that can be pushed.
    uint32_t *items; // Array of items, each with type uint32_t.
};

//The following function was taken from the assignment doc
//The function creates a stack with no items and a capacity
//based on the parameter defined. It also allocated
//memory for the stack. It returns the stack.

Stack *stack_create(uint32_t capacity) {
    Stack *s = (Stack *) malloc(sizeof(Stack));
    if (s) {
        s->top = 0;
        s->capacity = capacity;
        s->items = (uint32_t *) calloc(capacity, sizeof(uint32_t));
        if (!s->items) {
            free(s);
            s = NULL;
        }
    }
    return s;
}

//The following function was taken from the assignment doc
//This function deletes the stack and frees the memory it used.
//It returns nothing.

void stack_delete(Stack **s) {
    if (*s && (*s)->items) {
        free((*s)->items);
        free(*s);
        *s = NULL;
    }
    return;
}

//This function checks whether the stack that was
//input as a parameter. It returns true if it's empty,
//false otherwise.

bool stack_empty(Stack *s) {
    if (s->top == 0) {
        return true;
    }
    return false;
}

//This function checks if the stack is full, returning
//true if so and false otherwise. It takes a stack as its
//parameter.

bool stack_full(Stack *s) {
    if (s->top == s->capacity) {
        return true;
    }
    return false;
}

//This function takes in a stack as its parameter
//and returns the value top of the stack.

uint32_t stack_size(Stack *s) {
    return s->top;
}

//This function pushes an item onto the stack, it
//takes the item x as a parameter as well as the
//stack that it will be pushed on to. It returns
//true if it succeeds and false otherwise.

bool stack_push(Stack *s, uint32_t x) {
    if (!stack_full(s)) {
        s->items[s->top] = x;
        s->top++;
        return true;
    }
    return false;
}

//This function takes in two parameters x and s,
//where s is a stack and x is the pointer to
//a variable to which the popped value will be
//stored. It returns false if it fails and true
//if it succeeds.

bool stack_pop(Stack *s, uint32_t *x) {
    if (!stack_empty(s)) {
        s->top--;
        *x = s->items[s->top];
        return true;
    }
    return false;
}

//This function returns the most recently pushed
//value of the stack, returning true if it did so
//succesfully and false otherwise. It takes in two
//parameters, stack s and pointer to vertex x.

bool stack_peek(Stack *s, uint32_t *x) {
    if (!stack_empty(s)) {
        *x = s->items[s->top - 1];
        return true;
    }
    return false;
}

//This function copies over a stack from a source
//stack input as a parameter to a destination
//stack input as a parameter and returns nothing.

void stack_copy(Stack *dst, Stack *src) {
    if (src->capacity == dst->capacity) {
        for (uint32_t i = 0; i < src->top; i += 1) {
            dst->items[i] = src->items[i];
        }
        dst->top = src->top;
    }
    return;
}

//The following function was taken from the assignment doc
//The function prints out the contents of the stack.

void stack_print(Stack *s, FILE *outfile, char *cities[]) {
    for (uint32_t i = 0; i < s->top; i += 1) {
        fprintf(outfile, "%s", cities[s->items[i]]);
        if (i + 1 != s->top) {
            fprintf(outfile, " -> ");
        }
    }
    fprintf(outfile, "\n");
    return;
}
