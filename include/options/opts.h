//
//  opts.h
//  TareaIO
//
//  Created by Matias Barrientos on 09-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef opts_h
#define opts_h

#include "../enums.h"

typedef struct options options;

typedef enum metaheuristic metaheuristic;

extern options *opts;

enum metaheuristic {
    SA,
    TS,
    GENETIC,
    TBSA
};

struct options {
    const char *instance_file;
    metaheuristic mh;
    cooling_type cooling;
    move_type move;
    initial_solution initial;
    double alpha;
    double initial_temperature;
    double min_temperature;
};

void options_init (int argc, const char * argv[]);

#endif /* opts_h */
