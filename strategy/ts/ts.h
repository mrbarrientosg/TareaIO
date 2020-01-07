//
//  ts.h
//  TareaIO
//
//  Created by Matias Barrientos on 06-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef ts_h
#define ts_h

#include "../strategy.h"
#include "../../base/config.h"

typedef strategy ts;

typedef const struct ts_implementation ts_implementation;

typedef struct ts_base ts_base;

struct ts_base {
    const ts_implementation *ts_vtable;
};

struct ts_implementation {
    const strategy_implementation svtable;

    // Constructors
    ts * (*init) (ts_implementation *implementation, move_type move, initial_solution init_solution, long max_iterations, int max_tabus, int neighbors);
};

INLINE ts *
ts_init (ts_implementation *implementation, move_type move, initial_solution init_solution, long max_iterations, int max_tabus, int neighbors) {
    return implementation->init (implementation, move, init_solution, max_iterations, max_tabus, neighbors);
}

INLINE const state *
ts_solve (ts *ts) {
    return((const ts_base *) ts)->ts_vtable->svtable.solve (ts);
}


#endif /* ts_h */
