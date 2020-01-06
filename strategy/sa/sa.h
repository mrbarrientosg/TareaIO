//
//  sa_strategy.h
//  TareaIO
//
//  Created by Matias Barrientos on 06-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef sa_h
#define sa_h

#include "../strategy.h"
#include "../../base/config.h"

typedef strategy sa;

typedef const struct sa_implementation sa_implementation;

typedef struct sa_base sa_base;

typedef enum cooling_type cooling_type;

enum cooling_type {
    GEOMETRIC
};

struct sa_base {
    const sa_implementation *sa_vtable;
};

struct sa_implementation {
    const strategy_implementation svtable;

    // Constructors
    sa * (*init) (sa_implementation *implementation, cooling_type cooling, move_type move, double alpha);
    void (*setup) (sa *sa, double initial_temperature, double min_temperature, initial_solution init_solution);
};

extern const sa_implementation sa_strategy_implementation;

INLINE sa *
sa_init (sa_implementation *implementation,
         cooling_type cooling, move_type move, double alpha) {
    return implementation->init (implementation, cooling, move, alpha);
}

INLINE const state *
sa_solve (sa *sa, double initial_temperature, double min_temperature, initial_solution init_solution) {
    ((const sa_base *) sa)->sa_vtable->setup (sa, initial_temperature, min_temperature, init_solution);
    return((const sa_base *) sa)->sa_vtable->svtable.solve (sa);
}

#endif /* sa_h */
