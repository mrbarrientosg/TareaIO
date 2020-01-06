//
//  State.c
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#include "State.h"
#include "utils.h"
#include "alloc.h"
#include "debug_log.h"
#include <memory.h>

static void
initial_solution_greedy (State *state)
{
    int utilized = 0;
    int aux;
    int *ps, *pe, *pp = malloc(sizeof(int) * 2);
    //int gsize = 2;
    int *selected = NULL;
    ALLOC_N(selected, state->n);
    memset(selected, 0, sizeof(int) * state->n);

    while (utilized < state->n) {

        ps = pp;
        pe = pp + 1;
        aux = -1;

        for (int i = 0; i < state->n; i++) {
            if (selected[i] == 1)
                continue;

            for (int j = 0; j < i; j++) {
                if (selected[j] == 0 && srflp_weight(state->srflp, i, j) >= aux) {
                    aux = srflp_weight(state->srflp, i, j);
                    *ps = i;
                    *pe = j;
                }
            }
        }

        selected[*ps] = selected[*pe] = 1;

//        aux = 2;
//        while ((utilized + aux) < state->n && aux < 2) {
//            int pos = 0;
//            temp = -1;
//            int j = 0;
//
//            for (int i = 0; i < state->n; i++) {
//                if (*ps != i && selected[i] == 0 && srflp_weight(state->srflp, *ps, i) >= temp) {
//                    j = i;
//                    temp = srflp_weight(state->srflp, *ps, i);
//                    pos = 0;
//                }
//            }
//
//            for (int i = 0; i < state->n; i++) {
//                if (*pe != i && selected[i] == 0 && srflp_weight(state->srflp, *pe, i) >= temp) {
//                    j = i;
//                    temp = srflp_weight(state->srflp, *pe, i);
//                    pos = 1;
//                }
//            }
//
//            if (pos == 0) {
//                ps--;
//                *ps = j;
//            } else {
//                pe++;
//                *pe = j;
//            }
//            selected[j] = 1;
//            aux++;
//        }

        while (ps <= pe) {
            state->solution[utilized++] = *ps;
            ps++;
        }

        if (utilized == state->n - 1) {
            for (int i = 0; i < state->n; i++) {
                if (selected[i] == 0) {
                    state->solution[state->n - 1] = i;
                    selected[i] = 1;
                    utilized++;
                    break;
                }
            }
        }

    }

    FREE (selected);
    FREE (pp);
}

static void
initial_solution_deterministic (State *state) {
    for (int i = 0; i < state->n; i++) {
        state->solution[i] = i;
    }
}


State *
state_init (SRFLP * srflp, InitialSolution initial)
{
    State *state = NULL;

    if (ALLOC (state) == -1)
        FATAL_ERROR ("No hay memoria para reservar el objeto State.");

    state->srflp = srflp;
    state->n = srflp_problem_size (srflp);
    ALLOC_N (state->solution, state->n);

    switch (initial) {
        case DETERMINISTIC:
            initial_solution_deterministic (state);
            break;
        case GREEDY:
            initial_solution_greedy (state);
            break;
        default:
            break;
    }

    return state;
}

State *
state_init_copy (State *other)
{
    State *new = state_init (other->srflp, NONE);
    new->fitness = other->fitness;
    memmove (new->solution, other->solution, sizeof(int) * other->n);
    return new;
}

static State *
move_swap (State *state, int idx1, int idx2)
{
    State *new = state_init_copy (state);

    XORSWAP (new->solution[idx1], new->solution[idx2]);

    state_update_fitness (new);

    return new;
}

static State *
move_two_opt (State *state, int idx1, int idx2)
{
    State *new = state_init (state->srflp, NONE);

    int s = min(idx1, idx2);
    int e = max(idx1, idx2);

    memmove(new->solution, state->solution, sizeof(int) * s);

    int aux = 0;

    for (int i = s; i <= e; i++) {
        new->solution[i] = state->solution[e - aux];
        aux += 1;
    }

    memmove((new->solution + e + 1), (state->solution + e + 1), sizeof(int) * (state->n - (e + 1)));

    state_update_fitness (new);

    return new;
}

State *
state_random_neighbour (State *state, MoveType move)
{
    int idx1, idx2;

    idx1 = idx2 = random_number (&seed) % state->n;

    while (idx1 == idx2)
        idx2 = random_number (&seed) % state->n;

    switch (move) {
        case SWAP:
            return move_swap (state, idx1, idx2);
        case TWO_OPT:
            return move_two_opt (state, idx1, idx2);
    }
}

void
state_update_fitness (State *state)
{
    double total = 0.0;
    double middle_distance;
    int p1, p2;

    state->fitness = 0.0;

    for (int i = 0; i < (state->n - 1); i++) {
        p1 = state->solution[i];
        middle_distance = 0.0;
        for (int j = (i + 1); j < state->n; j++) {
            p2 = state->solution[j];
            total += (srflp_facility_size(state->srflp, p1)/2.0 + middle_distance + srflp_facility_size(state->srflp, p2)/2.0) * srflp_weight(state->srflp, p1, p2);
            middle_distance += srflp_facility_size(state->srflp, p2);
        }
    }

    state->fitness = total;
}

void
state_print (State *state)
{
    puts ("Solucion: ");
    for (int i = 0; i < state->n; i++) {
        printf ("%d ", state->solution[i]);
    }
    printf ("\nCosto: %f\n", state->fitness);
}

void
state_free (State **state)
{
    FREE ((*state)->solution);
    FREE (*state);
    *state = NULL;
}

