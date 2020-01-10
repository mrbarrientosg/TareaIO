#ifndef sa_h
#define sa_h

#include "strategy.h"
#include "../base/config.h"

typedef strategy sa;

typedef const struct sa_implementation sa_implementation;

typedef struct sa_base sa_base;

struct sa_base {
    const sa_implementation *sa_vtable;
};

struct sa_implementation {
    const strategy_implementation svtable;

    // Constructors
    sa * (*init) (sa_implementation *implementation);
};

INLINE sa *
sa_init (sa_implementation *implementation) {
    return implementation->init (implementation);
}

INLINE const state *
sa_solve (sa *sa) {
    return((const sa_base *) sa)->sa_vtable->svtable.solve (sa);
}

#endif /* sa_h */
