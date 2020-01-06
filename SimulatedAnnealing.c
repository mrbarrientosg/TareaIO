//
//  SimulatedAnnealing.c
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#include "SimulatedAnnealing.h"
#include "alloc.h"
#include "debug_log.h"
#include "utils.h"
#include "State.h"

#include <math.h>
#include <string.h>

SA *
sa_init (SRFLP *srflp, CoolingType cooling, MoveType move, double alpha)
{
    SA *sa = NULL;

    if (ALLOC (sa) == -1)
        FATAL_ERROR ("No se pudo asignar memoria para el objeto SA.");

    sa->srflp = srflp;
    sa->alpha = alpha;
    sa->cooling = cooling;
    sa->move = move;
    sa->best_solution = NULL;

    return sa;
}

static State *
next_state (SA *sa, State **current, double temperature, FILE *output)
{
    State *next = state_random_neighbour (*current, sa->move);

    double current_cost = (*current)->fitness;
    double next_cost = next->fitness;

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
reduce_temperature (SA *sa, double current_temp)
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

void
sa_search (SA *sa, State *initial, double initial_temp, double min_temp, const char *output)
{
    FILE *fp = NULL;
    if (output != NULL) {
        fp = fopen(output, "w");
        printif(fp, "iteration,temperature,current_fitness,acept_prob\n");
    }

    double temperature = initial_temp;

    State *current_state = state_init_copy (initial);

    state_update_fitness (current_state);

    sa->best_solution = state_init_copy (current_state);

    int iterations = 0;

    while (termination_condition (temperature, min_temp)) {
        printif(fp, "%d,%f,%f,", iterations, temperature, current_state->fitness);

        current_state = next_state (sa, &current_state, temperature, fp);

        if (current_state->fitness < sa->best_solution->fitness) {
            DEBUG_PRINT ("; mejor solucion actualizada\n");
            state_free (&sa->best_solution);
            sa->best_solution = state_init_copy (current_state);
        }

        temperature = reduce_temperature (sa, temperature);
        iterations += 1;
    }

    fclose (fp);
    state_free (&current_state);
}

