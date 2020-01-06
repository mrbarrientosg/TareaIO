//
//  main.c
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#include <stdio.h>
#include <time.h>
#include "SRFLP.h"
#include "utils.h"
#include "SimulatedAnnealing.h"

#define INSTANCE "/Users/matiasbarrientos/Downloads/Instancias Single Row Facility Layout Problem-20200102/QAP_sko56_04_n"

#define OUTPUT "/Users/matiasbarrientos/Desktop/temp.csv"

int main(int argc, const char * argv[]) {

    seed = (long int) time(NULL);

    SRFLP *srflp = srflp_init(INSTANCE);

    for (int i = 0; i < 1; i++) {
        State *s = state_init (srflp, GREEDY);
        //state_print (s);

        SA *sa = sa_init (srflp, GEOMETRIC, SWAP, 0.99999);
        sa_search (sa, s, 1000.0, 0.00001, NULL);
        //state_print (sa->best_solution);
        printf("%f,\n", sa->best_solution->fitness);
    }


    return 0;
}
