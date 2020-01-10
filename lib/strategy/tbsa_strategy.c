//
//  tbsa_strategy.c
//  TareaIO
//
//  Created by Matias Barrientos on 07-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#include "../../include/strategy/tbsa_strategy.h"
#include "../../include/base/debug_log.h"
#include "../../include/base/alloc.h"
#include "../../include/base/utils.h"
#include "../../include/base/list.h"
#include "../../include/base/map.h"
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <time.h>

struct strategy {
    tbsa_base base;
    double temperature;
    double min_temperature;
    long max_tabus;
    double alpha;
    cooling_type cooling;
    move_type move;
    initial_solution init_solution;
};

static tbsa *
tbsa_strategy_init (tbsa_implementation *implementation) {
    tbsa *tbsa = NULL;

    if (ALLOC (tbsa) == -1)
        FATAL_ERROR ("No se pudo asignar memoria para el objeto SA.");

    tbsa->base.tbsa_vtable = implementation;
    tbsa->move = opts->move;
    tbsa->init_solution = opts->initial;
    tbsa->temperature = opts->initial_temperature;
    tbsa->alpha = opts->alpha;
    tbsa->cooling = opts->cooling;
    tbsa->max_tabus = 2 * srflp_problem_size ();
    tbsa->min_temperature = opts->min_temperature;

    return tbsa;
}

static double
reduce_temperature (tbsa *tbsa, double current_temp)
{
    switch (tbsa->cooling) {
        case GEOMETRIC:
            return current_temp * tbsa->alpha;
    }
}

static int
termination_condition (double current_temp, double min_temp)
{
    return current_temp > min_temp;
}

static list *
get_neighbors(tbsa *tbsa, state *current) {
    list *l = list_init ((void *)state_free);
    int size = 2 * srflp_problem_size ();
    for (int i = 0; i < size; i++) {
        list_push_back (l, state_random_neighbour (current, tbsa->move));
    }

    return l;
}

//static int
//state_compare (state *s1, state *s2) {
//    return state_fitness(s1) - state_fitness(s2);
//}

static void 
remove_worst (map *tabu) {
    double min = DBL_MAX;
    state *it = map_first (tabu);

    while (it != NULL) {
        if (min > it->fitness) {
            min = it->fitness;
        }
        it = map_next (tabu);
    }

    map_remove_key (tabu, &min);
}

static long long
state_hash (state *state) {
    return (long long)state->fitness;
}

static const state *
tbsa_strategy_solve (tbsa *tbsa) {
    FILE *fp = fopen("best_solution.txt", "w");

    double temperature = tbsa->temperature;

    state *current_state = state_init (tbsa->init_solution);

    state_update_fitness (current_state);

    state *best = state_init_copy (current_state);

    map *tabu = map_init ((void *) state_hash, (void *) state_equals, (void *) state_free);

    unsigned long max_iter = 100 * srflp_problem_size ();

    unsigned long iterations = 0;

    unsigned long number_non_improv = 0;

    list *s_nei = NULL;

    clock_t begin = clock();

    while (termination_condition (temperature, tbsa->min_temperature) && iterations < max_iter) {

        if (number_non_improv) {
            temperature = tbsa->temperature;
            number_non_improv = 0;
            state_free (current_state);
            current_state = state_init (RANDOM);
            state_update_fitness (current_state);
            while (map_search_key(tabu, &current_state->fitness) != NULL && state_equals(current_state, best) != 1) {
                state_free (current_state);
                current_state = state_init (RANDOM);
                state_update_fitness (current_state);
            }
            map_release (&tabu);
            tabu = map_init ((void *) state_hash, (void *) state_equals, (void *) state_free);
            continue;
        }

        s_nei = get_neighbors (tbsa, current_state);

        state *candidate = list_first (s_nei);

        while (candidate != NULL) {
            if (map_search_key(tabu, &candidate->fitness) != NULL) {
                candidate = list_next (s_nei);
                continue;
            }

            double current_cost = state_fitness (current_state);
            double next_cost = state_fitness (candidate);

            if (next_cost < current_cost) {
                DEBUG_PRINT ("; mejor costo=%f\n", current_cost);
                map_insert (tabu, state_init_copy (current_state));

                if (map_size (tabu) > tbsa->max_tabus) {
                    remove_worst (tabu);
                }

                state_free (current_state);
                current_state = state_init_copy (candidate);
            } else {
                double prob = exp (-(next_cost - current_cost) / temperature);

                if (rnorm (&seed) < prob) {
                    DEBUG_PRINT ("; acepta peor costo=%f\n", next_cost);
                    map_insert (tabu, state_init_copy (current_state));

                    if (map_size (tabu) > tbsa->max_tabus) {
                        remove_worst (tabu);
                    }

                    state_free (current_state);
                    current_state = state_init_copy (candidate);
                } else {
                    map_insert (tabu, state_init_copy (current_state));

                    if (map_size (tabu) > tbsa->max_tabus) {
                        remove_worst (tabu);
                    }
                }
            }

            candidate = list_next (s_nei);
        }

        list_release (&s_nei);

        if (state_fitness (current_state) < state_fitness (best)) {
            DEBUG_PRINT ("; mejor solucion actualizada\n");
            state_free (best);
            best = state_init_copy (current_state);
            state_print (best, fp);
            rewind (fp);
        } else if (state_fitness (current_state) == state_fitness (best)) {
            number_non_improv = 1;
        } else {
            double error = fabs(state_fitness (best) - state_fitness (current_state)) / state_fitness (best);
            if (error < 0.00001) {
                number_non_improv = 1;
            }
        }

        iterations += 1;
        temperature = reduce_temperature (tbsa, temperature);
    }

    clock_t end = clock();

    double time = (end - begin) * 1000.0 / CLOCKS_PER_SEC;

    printf("\t - Execution time: %.4f ms\n", time);

    fclose (fp);
    map_release (&tabu);
    state_free (current_state);


    return best;
}

const struct tbsa_implementation tbsa_strategy_implementation = {
    {
        tbsa_strategy_solve
    },
    tbsa_strategy_init
};

