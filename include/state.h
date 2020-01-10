#ifndef state_h
#define state_h

#include "SRFLP.h"
#include "enums.h"
#include <stdio.h>

// Estado de la busqueda

typedef struct state state;

struct state {
    int n;
    double fitness;
    int *solution;
};

state *state_init (initial_solution initial);

state *state_init_copy (state *other);

state *state_random_neighbour (state *state, move_type move);

void state_update_fitness (state *state);

double state_fitness (state *state);

void state_set_solution (state *state, struct state *other);

state *move_swap (state *s, int idx1, int idx2);

void state_print (const state *state, FILE *fp);

int state_equals (const state *s1, const state *s2);

void state_free (state *state);

#endif /* state_h */
