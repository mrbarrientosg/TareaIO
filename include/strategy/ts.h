#ifndef ts_h
#define ts_h

#include "strategy.h"
#include "../base/config.h"

typedef strategy ts;

typedef const struct ts_implementation ts_implementation;

typedef struct ts_base ts_base;

struct ts_base {
    const ts_implementation *ts_vtable;
};

struct ts_implementation {
    const strategy_implementation svtable;

    // Constructors
    ts * (*init) (ts_implementation *implementation, int neighbors);
};

INLINE ts *
ts_init (ts_implementation *implementation, int neighbors) {
    return implementation->init (implementation, neighbors);
}

INLINE const state *
ts_solve (ts *ts) {
    return((const ts_base *) ts)->ts_vtable->svtable.solve (ts);
}


#endif /* ts_h */
