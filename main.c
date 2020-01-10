#include <stdio.h>
#include <time.h>
#include "include/options/opts.h"
#include "include/SRFLP.h"
#include "include/base/utils.h"
#include "include/base/debug_log.h"
#include "include/strategy/sa_strategy.h"
#include "include/strategy/ts_strategy.h"
#include "include/strategy/tbsa_strategy.h"
#include "include/strategy/genetic_strategy.h"

int main(int argc, const char * argv[]) {

    seed = (long int) time(NULL);

    options_init (argc, argv);

    srflp = srflp_init(opts->instance_file);

    switch (opts->mh) {
        case SA: {
            puts("Simulated Annealing:");
            sa *sa = sa_init(&sa_strategy_implementation);
            const state *best1 = sa_solve (sa);
            printf("\t - %.4f\n\n", best1->fitness);
            break;
        }
        case TS: {
            puts("Tabu Search:");
            ts *ts = ts_init(&ts_strategy_implementation, 20);
            const state *best1 = ts_solve(ts);
            printf("\t - %.4f\n\n", best1->fitness);
            break;
        }
        case GENETIC: {
            puts("Genetic:");
            genetic *genetic = genetic_init(&genetic_strategy_implementation);
            const state *best1 = genetic_solve(genetic);
            printf("\t - %.4f\n\n", best1->fitness);
            break;
        }
        case TBSA: {
            puts("Hybrid Tabu Search - Simulated Annealing:");
            tbsa *tbsa = tbsa_init(&tbsa_strategy_implementation);
            const state *best1 = tbsa_solve(tbsa);
            printf("\t - %.4f\n\n", best1->fitness);
            break;
        }
    }

// -------- SWAP ----------
//    Temperature: 1200.0
//    Min Temperature: 0.00001
//    Move: SWAP
//    Cooling: GEOMETRIC
//    Alpha: 0.9999
//    Execution time: 66.000000 seconds
//    Solucion:
//    40 14 20 24 53 0 15 3 45 1 7 36 47 33 38 25 26 16 44 5 19 11 42 30 46 52 49 27 23 54 13 6 35 9 10 28 50 4 43 55 12 17 51 8 39 22 29 41 48 32 18 21 31 2 37 34
//    Costo: 313388.000000

//    Temperature: 5000.0
//    Min Temperature: 0.00001
//    Move: SWAP
//    Cooling: GEOMETRIC
//    Alpha: 0.9999
//    Execution time: 72.000000 seconds
//    Solucion:
//    34 37 2 31 21 18 32 48 41 29 22 39 8 51 17 12 55 43 4 50 28 10 9 35 6 13 54 23 27 49 52 46 30 42 11 19 5 44 16 26 25 38 33 47 36 7 1 45 3 15 0 53 24 20 14 40
//    Costo: 313388.000000

// -------- TWO_OPT ----------
//    Temperature: 1200.0
//    Min Temperature: 0.00001
//    Move: TWO_OPT
//    Cooling: GEOMETRIC
//    Alpha: 0.9999
//    Execution time: 66.000000 seconds
//    Solucion:
//    14 20 24 53 37 1 3 15 32 21 45 7 55 25 5 44 42 41 8 23 54 22 52 49 30 16 50 29 35 10 6 9 19 28 11 27 13 26 43 12 51 4 33 47 36 17 46 38 39 48 18 31 0 2 34 40
//    Costo: 313497.000000

//    Temperature: 5000.0
//    Min Temperature: 0.00001
//    Move: TWO_OPT
//    Cooling: GEOMETRIC
//    Alpha: 0.9999
//    Execution time: 79.000000 seconds
//    Solucion:
//    40 14 20 24 53 0 15 3 45 1 7 36 47 33 38 25 26 16 11 44 19 5 42 30 46 52 49 27 23 54 13 6 35 9 10 28 50 4 43 55 12 17 51 8 39 22 29 41 48 32 18 21 31 2 37 34
//    Costo: 313397.000000


// -------- OTRAS METAHEURISTICAS --------

//    ts *ts = ts_init(&ts_strategy_implementation, SWAP, GREEDY, 10);
//    const state *best2 = ts_solve (ts);
//    state_print (best2, NULL);

//    Solucion:
//    40 34 2 37 21 31 18 48 32 41 39 22 29 8 50 51 17 12 43 46 28 9 35 13 27 6 10 52 23 49 54 42 30 4 11 19 5 26 44 16 55 25 33 47 38 45 7 36 1 3 15 0 53 24 20 14
//    Costo: 313701.000000

//    tbsa *tbsa = tbsa_init(&tbsa_strategy_implementation, SWAP, GREEDY);
//    const state *best3 = tbsa_solve (tbsa);
//    state_print (best3, NULL);

//    genetic *genetic = genetic_init(&genetic_strategy_implementation, TWO_OPT, GREEDY);
//    const state *best3 = genetic_solve (genetic);
//    state_print (best3, NULL);

//    Solucion:
//    14 20 24 53 37 21 32 15 1 3 45 7 25 5 55 44 42 22 23 54 8 16 50 49 52 30 35 9 6 10 29 19 28 27 13 11 26 51 43 4 12 33 47 36 17 46 41 38 39 48 18 31 0 2 34 40
//    Costo: 313763.000000

    return 0;
}
