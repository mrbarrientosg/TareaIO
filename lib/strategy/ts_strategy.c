#include "../../include/strategy/ts_strategy.h"
#include "../../include/base/debug_log.h"
#include "../../include/base/alloc.h"
#include "../../include/base/utils.h"
#include "../../include/base/queue.h"
#include "../../include/base/list.h"
#include <time.h>

struct strategy {
    ts_base base;
    long max_iterations;
    int max_tabus;
    int neighbors;
    move_type move;
    initial_solution init_solution;
};

static ts *
ts_strategy_init (ts_implementation *implementation, int neighbors) {
    ts *ts = NULL;

    if (ALLOC (ts) == -1)
        FATAL_ERROR ("No se pudo asignar memoria para el objeto SA.");

    ts->base.ts_vtable = implementation;
    ts->move = opts->move;
    ts->max_iterations = 10000 * srflp_problem_size();
    ts->init_solution = opts->initial;
    ts->max_tabus = 2 * srflp_problem_size ();
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

    long stop_turn = 0;

    long limit = 10 * srflp_problem_size ();

    clock_t begin = clock();

    while (iterations <= ts->max_iterations) {
        s_nei = get_neighbors (ts, current_state);
        state_free (current_state);

        current_state = state_init_copy (list_first (s_nei));

        state *candidate = list_first (s_nei);

        while (candidate != NULL) {
            if (!queue_contains (tabuQueue, candidate, (void *) state_equals) && state_fitness (candidate) < state_fitness (current_state)) {
                state_free (current_state);
                current_state = state_init_copy (candidate);
            }

            candidate = list_next (s_nei);
        }

        list_release (&s_nei);

        if (state_fitness (current_state) < state_fitness (best)) {
            state_free (best);
            best = state_init_copy (current_state);
            stop_turn = 0;
        }

        queue_push (tabuQueue, state_init_copy (current_state));

        if (queue_size (tabuQueue) > ts->max_tabus) {
            queue_pop (tabuQueue);
        }

        if (stop_turn == limit) {
            state_free (current_state);
            current_state = state_init (RANDOM);
            state_update_fitness (current_state);
            stop_turn = 0;
            queue_release (&tabuQueue);
            tabuQueue = queue_init ((void *)state_free);
            continue;
        }

        stop_turn += 1;
        iterations += 1;
    }

    clock_t end = clock();

    double time = (end - begin) * 1000.0 / CLOCKS_PER_SEC;

    printf("\t - Execution time: %.4f ms\n", time);

    queue_release (&tabuQueue);
    state_free (current_state);

    return best;
}

const struct ts_implementation ts_strategy_implementation = {
    {
        ts_strategy_solve
    },
    ts_strategy_init
};
