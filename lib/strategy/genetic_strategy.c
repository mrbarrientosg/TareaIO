//
//  genetic_strategy.c
//  TareaIO
//
//  Created by Matias Barrientos on 08-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#include "../../include/strategy/genetic_strategy.h"
#include "../../include/base/debug_log.h"
#include "../../include/base/alloc.h"
#include "../../include/base/utils.h"
#include "../../include/base/queue.h"
#include "../../include/base/list.h"
#include <time.h>

struct strategy {
    genetic_base base;
    long max_iterations;
    int max_individuals;
    move_type move;
    initial_solution init_solution;
};

static genetic *
genetic_strategy_init (genetic_implementation *implementation) {
    genetic *genetic = NULL;

    if (ALLOC (genetic) == -1)
        FATAL_ERROR ("No se pudo asignar memoria para el objeto SA.");

    genetic->base.gvtable = implementation;
    genetic->move = opts->move;
    genetic->max_iterations = 50;
    genetic->init_solution = opts->initial;
    genetic->max_individuals = 30;

    return genetic;
}


static void
generate_initial_poblation(genetic *genetic, list *poblation) {
    for (int i = 0; i < genetic->max_individuals; i++) {
        state *new = state_init (RANDOM);
        state_update_fitness (new);
        list_push_back (poblation, new);
    }
}

static void
ordenar_bacterias(genetic *genetic, list *poblation){
    int orden = 0;
    int i = 0;
    state *aux = NULL;
    state *iterator;
    state *iterator_next;

    while(i < list_size (poblation) && orden == 0){
        i += 1;
        orden = 1;
        iterator = list_first (poblation);
        for (int j = 0; j < list_size (poblation) - 1; j++, iterator = list_next (poblation)) {
            iterator_next = list_next (poblation);
            list_prev (poblation);

            if (state_fitness (iterator_next) < state_fitness (iterator)) {
                orden = 0;
                aux = state_init_copy (iterator);
                state_set_solution (iterator, iterator_next);
                iterator->fitness = iterator_next->fitness;

                state_set_solution (iterator_next, aux);
                iterator_next->fitness = aux->fitness;

                state_free (aux);
            }
        }
    }
}

static const state *
genetic_strategy_solve (genetic *genetic) {

    list *poblation = list_init ((void *)state_free);

    generate_initial_poblation (genetic, poblation);

    ordenar_bacterias (genetic, poblation);

    state *best = NULL;

    int iterations = 0;

    state *iterator;

    clock_t begin = clock();

    while (iterations < genetic->max_iterations) {

        iterator = list_first (poblation);

        while (iterator != NULL) {
            for (int i = 0; i < srflp_problem_size () - 1; i++) {
                for(int j = i + 1 ; j < srflp_problem_size () ; j++){
                    state *mutate = move_swap (iterator, i, j);

                    if (state_fitness (mutate) < state_fitness (iterator)) {
                        state_set_solution (iterator, mutate);
                        state_update_fitness (iterator);
                    }

                    state_free (mutate);
                }
            }
            iterator = list_next (poblation);
        }

        ordenar_bacterias (genetic, poblation);

        if (best == NULL || state_fitness ((state *) list_first (poblation)) < state_fitness (best)) {
            state_free (best);
            best = state_init_copy(list_first (poblation));
        }

        iterations += 1;
    }

    clock_t end = clock();

    double time = (end - begin) * 1000.0 / CLOCKS_PER_SEC;

    printf("\t - Execution time: %.4f ms\n", time);

//    iterator = list_first (poblation);
//
//    while (iterator) {
//        state_print (iterator, NULL);
//        iterator = list_next (poblation);
//    }

    list_release (&poblation);

    return best;
}

const struct genetic_implementation genetic_strategy_implementation = {
    {
        genetic_strategy_solve
    },
    genetic_strategy_init
};

