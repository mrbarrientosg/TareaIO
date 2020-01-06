//
//  SRFLP.h
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#ifndef SRFLP_h
#define SRFLP_h

typedef struct SRFLP SRFLP;

#include "State.h"

SRFLP *srflp_init (const char *filename);

void srflp_print_instance (SRFLP *srflp);

int srflp_problem_size (SRFLP *srflp);

int srflp_weight (SRFLP *srflp, int i, int j);

int srflp_facility_size (SRFLP *srflp, int i);

#endif /* SRFLP_h */
