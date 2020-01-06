//
//  SRFLP.c
//  TareaIO
//
//  Created by Matias Barrientos on 02-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#include "SRFLP.h"
#include "../base/alloc.h"
#include "../base/debug_log.h"
#include "../base/utils.h"

#include <stdio.h>
#include <string.h>

SRFLP *srflp = NULL;

struct SRFLP {
    int n;
    int *f_size;
    int **f_weight;
    char *name;
};

static void
read_srflp (SRFLP *srflp, const char *filename)
{
    if (filename == NULL)
        FATAL_ERROR ("Instancia no especificada.");

    FILE *fp = fopen (filename, "r");
    int i = -1;

    if (fp == NULL) {
        char m[FILENAME_MAX] = "No se puede leer el archivo: ";
        FATAL_ERROR (strcat (m, filename));
    }

    printf ("\nLeyendo archivo %s ... \n", filename);

    char *line = read_line (fp);

    while (line != NULL) {
        if (strstr (line, "EOF") != NULL)
            break;

        if (line[0] == '#') {
            FREE (line);
            line = read_line (fp);
            continue;
        }

        if (srflp->n < 0) {
            srflp->n = atoi(line);
            ASSERT ((ALLOC_N (srflp->f_size, srflp->n) != -1), "No hay memoria para asignar.");
            ASSERT ((ALLOC_N (srflp->f_weight, srflp->n) != -1), "No hay memoria para asignar.");

            for (int  k = 0; k < srflp->n; k++) {
                ALLOC_N (srflp->f_weight[k], srflp->n);
            }
        } else if (i == -1) {
            char *token = strtok (line, ",");
            int k = 0;

            while (token != NULL && k < srflp->n) {
                srflp->f_size[k] = atoi (token);
                token = strtok (NULL, ",");
                k += 1;
            }

            i = 0;
        } else {
            char *token = strtok (line, ",");
            int k = 0;

            while (token != NULL && k < srflp->n) {
                srflp->f_weight[i][k] = atoi (token);
                token = strtok (NULL, ",");
                k += 1;
            }
            i += 1;
        }

        FREE (line);
        line = read_line (fp);
    }
}

SRFLP *
srflp_init (const char *filename)
{
    SRFLP *srflp = NULL;

    if (ALLOC (srflp) == -1)
        FATAL_ERROR ("No hay memoria para asignar al objeto SRFLP.");

    srflp->n = -1;
    srflp->f_size = NULL;
    srflp->f_weight = NULL;
    srflp->name = strdup(filename);

    read_srflp (srflp, filename);

    return srflp;
}

void
srflp_print_instance (SRFLP *srflp)
{
    printf ("Numero de instalaciones: %d\n", srflp->n);
    printf ("Tamaños: ");
    for (int k = 0; k < srflp->n; k++) {
        printf ("%d ", srflp->f_size[k]);
    }
    printf ("\n");
    printf ("Pesos: \n");
    for (int i = 0; i < srflp->n; i++) {
        for (int k = 0; k < srflp->n; k++) {
            printf ("%d ", srflp->f_weight[i][k]);
        }
        printf ("\n");
    }
}

int
srflp_weight (SRFLP *srflp, int i, int j)
{
    if (i >= srflp->n || i < 0 || j >= srflp->n || j < 0)
        FATAL_ERROR ("Indice fuera de rango.");

    return srflp->f_weight[i][j];
}

int
srflp_facility_size (SRFLP *srflp, int i)
{
    if (i >= srflp->n || i < 0)
        FATAL_ERROR ("Indice fuera de rango.");

    return srflp->f_size[i];
}

int
srflp_problem_size (SRFLP *srflp)
{
    return srflp->n;
}
