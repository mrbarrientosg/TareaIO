//
//  tbsa.h
//  TareaIO
//
//  Created by Matias Barrientos on 07-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef tbsa_h
#define tbsa_h

#include "strategy.h"
#include "../base/config.h"

typedef strategy tbsa;

typedef const struct tbsa_implementation tbsa_implementation;

typedef struct tbsa_base tbsa_base;

struct tbsa_base {
    const tbsa_implementation *tbsa_vtable;
};

struct tbsa_implementation {
    const strategy_implementation svtable;

    // Constructors
    tbsa * (*init) (tbsa_implementation *implementation);
};

INLINE tbsa *
tbsa_init (tbsa_implementation *implementation) {
    return implementation->init (implementation);
}

INLINE const state *
tbsa_solve (tbsa *tbsa) {
    return((const tbsa_base *) tbsa)->tbsa_vtable->svtable.solve (tbsa);
}

#endif /* tbsa_h */
