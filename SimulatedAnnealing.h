//
//  SimulatedAnnealing.h
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef SimulatedAnnealing_h
#define SimulatedAnnealing_h

#include "SRFLP.h"

typedef struct SA SA;

typedef enum CoolingType CoolingType;

enum CoolingType {
    GEOMETRIC
};

struct SA {
    double alpha;
    CoolingType cooling;
    MoveType move;
    SRFLP *srflp;
    State *best_solution;
};

SA *sa_init (SRFLP *srflp, CoolingType cooling, MoveType move, double alpha);

void sa_search (SA *sa, State *initial, double initial_temp, double min_temp, const char *output);

#endif /* SimulatedAnnealing_h */
