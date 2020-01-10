#ifndef strategy_h
#define strategy_h

typedef const struct strategy_implementation strategy_implementation;

typedef struct strategy strategy;

#include "../state.h"
#include "../SRFLP.h"
#include "../options/opts.h"
#include "../enums.h"

struct strategy_implementation {
    const state *(*solve)(strategy *strategy);
};

#endif /* strategy_h */
