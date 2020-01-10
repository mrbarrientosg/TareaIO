#ifndef SRFLP_h
#define SRFLP_h

typedef struct SRFLP SRFLP;

extern SRFLP *srflp;

SRFLP *srflp_init (const char *filename);

void srflp_print_instance (void);

int srflp_problem_size (void);

int srflp_weight (int i, int j);

int srflp_facility_size (int i);

#endif /* SRFLP_h */
