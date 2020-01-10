//
//  opts.c
//  TareaIO
//
//  Created by Matias Barrientos on 09-01-20.
//  Copyright © 2020 Matias Barrientos. All rights reserved.
//

#include "opts.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../base/debug_log.h"
#include "../base/alloc.h"
#include "../base/utils.h"
#include "../base/map.h"

options *opts = NULL;

typedef struct _options _options;

typedef struct all_options all_options;

struct all_options {
    map *short_names;
    map *long_names;
};

struct _options {
    char *name;
    char *short_name;
    char *long_name;
    char *description;
    int has_arg: 1;
};

static void
_options_free (_options *opt) {
    FREE(opt->name);
    FREE(opt->short_name);
    FREE(opt->long_name);
    FREE(opt->description);
    FREE(opt);
}

static _options *
_options_init (const char *name, const char *short_name, const char *long_name, int has_arg, const char *description) {
    _options *opt = NULL;

    ALLOC(opt);

    opt->name = strdup (name);
    opt->short_name = strdup (short_name);
    opt->long_name = strdup (long_name);
    opt->description = strdup (description);
    opt->has_arg = has_arg;

    return opt;
}

static long long
short_name_hash(_options *opt) {
    long long hash = 5381;

    const char *ptr = opt->short_name;

    while (*ptr) {
        hash = ((hash << 5) + hash) + tolower(*ptr);
        ptr++;
    }

    return hash;
}

static int
short_name_equal(_options *key1, _options *key2) {
    const char *A = key1->short_name;
    const char *B = key2->short_name;

    return strcmp(A, B) == 0;
}

static long long
long_name_hash(_options *opt) {
    long long hash = 5381;

    const char *ptr = opt->long_name;

    while (*ptr) {
        hash = ((hash << 5) + hash) + tolower(*ptr);
        ptr++;
    }

    return hash;
}

static int
long_name_equal(_options *key1, _options *key2) {
    const char *A = key1->long_name;
    const char *B = key2->long_name;

    return strcmp(A, B) == 0;
}

static void
print_help_message (map *options) {
    _options *it = map_first (options);

    puts ("Runner [OPTION]... [ARGUMENT]...");

    while (it != NULL) {
        int size = 14 + (int)strlen(it->long_name) + (int)strlen(it->short_name);
        printf("\t%s  %s%*c%s\n", it->short_name, it->long_name, abs(42 - size) , ' ', it->description);

        it = map_next (options);
    }
    puts("\n");

    exit (0);
}

static all_options *
all_options_init () {
    map *short_names = map_init((void *)short_name_hash, (void *)short_name_equal, (void *) _options_free);
    map_insert (short_names, _options_init ("help", "-h", "--help", 0, "Imprime este mensaje"));
    map_insert (short_names, _options_init ("instanceFile", "-i", "--instance-file", 1, "Ruta archivo que contiene la instancia"));
    map_insert (short_names, _options_init ("metaheuristic", "-m", "--metaheuristic", 1, "Metaheuristica para solucionar el problema"));
    map_insert (short_names, _options_init ("move", "-mm", "--move", 1, "Tipo de movimiento"));
    map_insert (short_names, _options_init ("alpha", "-a", "--alpha", 1, "Coeficiente de enfriamiento"));
    map_insert (short_names, _options_init ("initialTemperature", "-t0", "--initial-temperature", 1, "Temperatura inicial"));
    map_insert (short_names, _options_init ("minTemperature", "-tmin", "--min-temperature", 1, "Temperatura minima"));
    map_insert (short_names, _options_init ("initialSolution", "-is", "--initial-solution", 1, "Solucion inicial con la que el problema empieza"));


    all_options *all_opts = NULL;

    ALLOC (all_opts);
    all_opts->short_names = short_names;
    all_opts->long_names = map_init((void *)long_name_hash, (void *)long_name_equal, (void *) _options_free);

    _options *it = map_first (short_names);

    while (it != NULL) {
        map_insert (all_opts->long_names, it);
        it = map_next (short_names);
    }

    return all_opts;
}

void
options_init (int argc, const char * argv[]) {

    all_options *options = all_options_init ();

    if (argc < 2 || !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help")) {
        print_help_message (options->short_names);
    }

    ALLOC (opts);
    opts->alpha = 0.99;
    opts->cooling = GEOMETRIC;
    opts->initial_temperature = 1000.0;
    opts->mh = SA;
    opts->initial = GREEDY;
    opts->move = SWAP;
    opts->min_temperature = 1.0;

    int idx = 1;

    _options *aux = NULL;
    _options *search = NULL;

    while (idx < (argc - 1)) {
        if (strstr(argv[idx], "--")) {
            aux = _options_init (NULL, NULL, argv[idx], 0, NULL);
            search = map_search_key (options->long_names, aux);
        } else {
            aux = _options_init (NULL, argv[idx], NULL, 0, NULL);
            search = map_search_key (options->short_names, aux);
        }

        if (!strcmp (search->name, "instanceFile")) {
            opts->instance_file = argv[++idx];
        }

        if (!strcmp (search->name, "metaheuristic")) {
            if (!strcmp (argv[idx+1], "sa")) {
                opts->mh = SA;
            } else if (!strcmp (argv[idx+1], "ts")) {
                opts->mh = TS;
            } else if (!strcmp (argv[idx+1], "genetic")) {
                opts->mh = GENETIC;
            } else if (!strcmp (argv[idx+1], "tbsa")) {
                opts->mh = TBSA;
            } else {
                ERROR ("Esa metaheuristica no existe.");
            }
            idx += 1;
        }

        if (!strcmp (search->name, "move")) {
            if (!strcmp (argv[idx+1], "swap")) {
                opts->move = SWAP;
            } else if (!strcmp (argv[idx+1], "2opt")) {
                opts->move = TWO_OPT;
            } else {
                ERROR ("Ese movimiento no existe.");
            }
            idx += 1;
        }

        if (!strcmp (search->name, "initialSolution")) {
            if (!strcmp (argv[idx+1], "greedy")) {
                opts->initial = GREEDY;
            } else if (!strcmp (argv[idx+1], "random")) {
                opts->initial = RANDOM;
            } else if (!strcmp (argv[idx+1], "deterministic")) {
                opts->initial = DETERMINISTIC;
            } else {
                ERROR ("No existe ese algoritmo de solucion inicial.");
            }
            idx += 1;
        }

        if (!strcmp (search->name, "alpha")) {
            opts->alpha = atof(argv[++idx]);
        }

        if (!strcmp (search->name, "initialTemperature")) {
            opts->initial_temperature = atof(argv[++idx]);
        }

        if (!strcmp (search->name, "minTemperature")) {
            opts->min_temperature = atof(argv[++idx]);
        }
        
        idx += 1;
        _options_free (aux);
    }
}
