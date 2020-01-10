//
//  genetic.h
//  TareaIO
//
//  Created by Matias Barrientos on 08-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef genetic_h
#define genetic_h

#include "strategy.h"
#include "../base/config.h"

typedef strategy genetic;

typedef const struct genetic_implementation genetic_implementation;

typedef struct genetic_base genetic_base;

struct genetic_base {
    const genetic_implementation *gvtable;
};

struct genetic_implementation {
    const strategy_implementation svtable;

    // Constructors
    genetic * (*init) (genetic_implementation *implementation);
};

INLINE genetic *
genetic_init (genetic_implementation *implementation) {
    return implementation->init (implementation);
}

INLINE const state *
genetic_solve (genetic *genetic) {
    return((const genetic_base *) genetic)->gvtable->svtable.solve (genetic);
}


#endif /* genetic_h */
