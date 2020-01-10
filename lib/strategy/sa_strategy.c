#include "../../include/strategy/sa_strategy.h"
#include "../../include/base/alloc.h"
#include "../../include/base/debug_log.h"
#include "../../include/base/utils.h"
#include "../../include/base/heap.h"
#include <math.h>
#include <time.h>

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
sa_strategy_init (sa_implementation *implementation) {
    sa *sa = NULL;

    if (ALLOC (sa) == -1)
        FATAL_ERROR ("No se pudo asignar memoria para el objeto SA.");

    sa->base.sa_vtable = implementation;
    sa->alpha = opts->alpha;
    sa->cooling = opts->cooling;
    sa->move = opts->move;
    sa->temperature = opts->initial_temperature;
    sa->min_temperature = opts->min_temperature;
    sa->init_solution = opts->initial;
    
    return sa;
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
        state_free (*current);
        return next;
    } else {
        double prob = exp (-(next_cost - current_cost) / temperature);
        printif(output, "%f\n", prob);

        if (rnorm (&seed) <= prob) {
            DEBUG_PRINT ("; acepta peor costo=%f\n", next_cost);
            state_free (*current);
            return next;
        } else {
            DEBUG_PRINT ("; no acepta peor costo=%f\n; solucion actual costo=%f\n", next_cost, current_cost);
            state_free (next);
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
    FILE *fp = NULL; // output

    double temperature = sa->temperature;

    state *current_state = state_init (sa->init_solution);

    state_update_fitness (current_state);

    state *best = state_init_copy (current_state);

    long iterations = 0;

    long stop_run = 0;

    long limit = 10 * srflp_problem_size ();

    clock_t begin = clock();

    while (termination_condition (temperature, sa->min_temperature)) {
        printif(fp, "%ld,%f,%f,", iterations, temperature, best->fitness);

        current_state = next_state (sa, &current_state, temperature, fp);

        if (state_fitness (current_state) < state_fitness (best)) {
            DEBUG_PRINT ("; mejor solucion actualizada\n");
            state_free (best);
            best = state_init_copy (current_state);
            stop_run = 0;
        }


        if (stop_run == limit) {
            temperature = sa->temperature;
            stop_run = 0;
            limit *= 1.1;
            state_free (current_state);
            current_state = state_init (RANDOM);
            state_update_fitness (current_state);
            continue;
        }

        temperature = reduce_temperature (sa, temperature);
        iterations += 1;
        stop_run += 1;
    }

    clock_t end = clock();

    double time = (end - begin) * 1000.0 / CLOCKS_PER_SEC;

    printf("\t - Execution time: %.4f ms\n", time);

    state_free (current_state);

    return best;
}

const struct sa_implementation sa_strategy_implementation = {
    {
        sa_strategy_solve
    },
    sa_strategy_init
};
