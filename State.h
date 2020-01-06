//
//  State.h
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef State_h
#define State_h

typedef struct State State;

#include "SRFLP.h"

typedef enum InitialSolution InitialSolution;
typedef enum MoveType MoveType;

enum MoveType {
    SWAP,
    TWO_OPT
};

enum InitialSolution {
    DETERMINISTIC,
    GREEDY,
    NONE
};

struct State {
    SRFLP *srflp;
    int n;
    double fitness;
    int *solution;
};

State *state_init (SRFLP * srflp, InitialSolution initial);

State *state_init_copy (State *other);

State *state_random_neighbour (State *state, MoveType move);

void state_update_fitness (State *state);

void state_print (State *state);

void state_free (State **state);

#endif /* State_h */
