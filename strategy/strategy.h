//
//  solver.h
//  TareaIO
//
//  Created by Matias Barrientos on 06-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef strategy_h
#define strategy_h

typedef const struct strategy_implementation strategy_implementation;

typedef struct strategy strategy;

#include "../core/state.h"
#include "../core/SRFLP.h"

struct strategy_implementation {
    const state *(*solve)(strategy *strategy);
};

#endif /* strategy_h */
