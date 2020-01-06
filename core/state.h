//
//  state.h
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef state_h
#define state_h

#include "SRFLP.h"

typedef struct state state;

typedef enum initial_solution initial_solution;

typedef enum move_type move_type;

enum move_type {
    SWAP,
    TWO_OPT
};

enum initial_solution {
    DETERMINISTIC,
    GREEDY,
    NONE
};

state *state_init (initial_solution initial);

state *state_init_copy (state *other);

state *state_random_neighbour (state *state, move_type move);

void state_update_fitness (state *state);

double state_fitness (state *state);

void state_print (const state *state);

void state_free (state **state);

#endif /* state_h */
