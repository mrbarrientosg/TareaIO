//
//  ts_strategy.c
//  TareaIO
//
//  Created by Matias Barrientos on 06-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#include "ts_strategy.h"
#include "../../base/debug_log.h"
#include "../../base/alloc.h"
#include "../../base/utils.h"
#include "../../base/queue.h"
#include "../../base/list.h"

struct strategy {
    ts_base base;
    long max_iterations;
    int max_tabus;
    int neighbors;
    move_type move;
    initial_solution init_solution;
};

static ts *
ts_strategy_init (ts_implementation *implementation, move_type move, initial_solution init_solution, long max_iterations, int max_tabus, int neighbors) {
    ts *ts = NULL;

    if (ALLOC (ts) == -1)
        FATAL_ERROR ("No se pudo asignar memoria para el objeto SA.");

    ts->base.ts_vtable = implementation;
    ts->move = move;
    ts->max_iterations = max_iterations;
    ts->init_solution = init_solution;
    ts->max_tabus = max_tabus;
    ts->neighbors = neighbors;

    return ts;
}

static list *
get_neighbors(ts *ts, state *current) {
    list *l = list_init ((void *)state_free);

    for (int i = 0; i < ts->neighbors; i++) {
        list_push_back (l, state_random_neighbour (current, ts->move));
    }

    return l;
}


static const state *
ts_strategy_solve (ts *ts) {

    state *current_state = state_init (ts->init_solution);

    state_update_fitness (current_state);

    state *best = state_init_copy (current_state);

    int iterations = 0;

    queue *tabuQueue = queue_init ((void *)state_free);

    list *s_nei = NULL;

    while (iterations <= ts->max_iterations) {
        s_nei = get_neighbors (ts, current_state);
        state_free (current_state);

        current_state = state_init_copy (list_first (s_nei));

        state *candidate = list_first (s_nei);

        while (candidate != NULL) {
            if (!queue_contains (tabuQueue, current_state, (void *) state_equals) && state_fitness (candidate) < state_fitness (current_state)) {
                state_free (current_state);
                current_state = state_init_copy (candidate);
            }

            candidate = list_next (s_nei);
        }

        if (state_fitness (current_state) < state_fitness (best)) {
            state_free (best);
            best = state_init_copy (current_state);
        }

        queue_push (tabuQueue, state_init_copy (current_state));

        if (queue_size (tabuQueue) > ts->max_tabus) {
            queue_pop (tabuQueue);
        }

        list_release (&s_nei);
        iterations += 1;
    }

    queue_release (&tabuQueue);

//    while (termination_condition (temperature, sa->min_temperature)) {
//        printif(NULL, "%d,%f,%f,", iterations, temperature, state_fitness (current_state));
//
//        current_state = next_state (sa, &current_state, temperature, NULL);
//
//        if (state_fitness (current_state) < state_fitness (best)) {
//            DEBUG_PRINT ("; mejor solucion actualizada\n");
//            state_free (&best);
//            best = state_init_copy (current_state);
//        }
//
//        temperature = reduce_temperature (sa, temperature);
//        iterations += 1;
//    }

    state_free (current_state);

    return best;
}

const struct ts_implementation ts_strategy_implementation = {
    {
        ts_strategy_solve
    },
    ts_strategy_init
};
