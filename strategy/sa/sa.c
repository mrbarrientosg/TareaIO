//
//  sa_strategy.c
//  TareaIO
//
//  Created by Matias Barrientos on 06-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//
#ifndef INLINE
#define INLINE
#endif

#include "sa.h"
#include "alloc.h"
#include "debug_log.h"
#include "utils.h"

#include <math.h>

struct strategy {
    sa_base base;
    double alpha;
    double temperature;
    double min_temperature;
    initial_solution init_solution;
    cooling_type cooling;
    move_type move;
};

static sa *
sa_strategy_init (sa_implementation *implementation, cooling_type cooling, move_type move, double alpha) {
    sa *sa = NULL;

    if (ALLOC (sa) == -1)
        FATAL_ERROR ("No se pudo asignar memoria para el objeto SA.");

    sa->base.sa_vtable = implementation;
    sa->alpha = alpha;
    sa->cooling = cooling;
    sa->move = move;

    return sa;
}

static void
sa_strategy_setup (sa *sa, double initial_temperature, double min_temperature, initial_solution init_solution) {
    sa->temperature = initial_temperature;
    sa->min_temperature = min_temperature;
    sa->init_solution = init_solution;
}

static state *
next_state (sa *sa, state **current, double temperature, FILE *output)
{
    state *next = state_random_neighbour (*current, sa->move);

    double current_cost = state_fitness (*current);
    double next_cost = state_fitness (next);

    if (next_cost < current_cost) {
        DEBUG_PRINT ("; mejor costo=%f\n", current_cost);
        printif(output, "NA\n");
        state_free (current);
        return next;
    } else {
        double prob = exp (-(next_cost - current_cost) / temperature);
        printif(output, "%f\n", prob);

        if (rnorm (&seed) <= prob) {
            DEBUG_PRINT ("; acepta peor costo=%f\n", next_cost);
            state_free (current);
            return next;
        } else {
            DEBUG_PRINT ("; no acepta peor costo=%f\n; solucion actual costo=%f\n", next_cost, current_cost);
            state_free (&next);
            return *current;
        }
    }
}

static double
reduce_temperature (sa *sa, double current_temp)
{
    switch (sa->cooling) {
        case GEOMETRIC:
            return current_temp * sa->alpha;
    }
}

static int
termination_condition (double current_temp, double min_temp)
{
    return current_temp > min_temp;
}

static const state *
sa_strategy_solve (sa *sa) {
    double temperature = sa->temperature;

    state *current_state = state_init (sa->init_solution);

    state_update_fitness (current_state);

    state *best = state_init_copy (current_state);

    int iterations = 0;

    while (termination_condition (temperature, sa->min_temperature)) {
        printif(NULL, "%d,%f,%f,", iterations, temperature, state_fitness (current_state));

        current_state = next_state (sa, &current_state, temperature, NULL);

        if (state_fitness (current_state) < state_fitness (best)) {
            DEBUG_PRINT ("; mejor solucion actualizada\n");
            state_free (&best);
            best = state_init_copy (current_state);
        }

        temperature = reduce_temperature (sa, temperature);
        iterations += 1;
    }

    state_free (&current_state);

    return best;
}

const struct sa_implementation sa_strategy_implementation = {
    {
        sa_strategy_solve
    },
    sa_strategy_init,
    sa_strategy_setup
};
