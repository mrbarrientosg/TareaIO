#include "../include/state.h"
#include "../include/base/utils.h"
#include "../include/base/alloc.h"
#include "../include/base/debug_log.h"
#include <memory.h>
#include <float.h>

static void
initial_solution_greedy (state *state)
{
    int utilized = 0;
    int aux;
    int *ps, *pe, *pp = malloc(sizeof(int) * 2);
    //int gsize = 2;
    int *selected = NULL;
    ALLOC_N (selected, state->n);
    memset (selected, 0, sizeof(int) * state->n);

    while (utilized < state->n) {

        ps = pp;
        pe = pp + 1;
        aux = -1;

        for (int i = 0; i < state->n; i++) {
            if (selected[i] == 1)
                continue;

            for (int j = 0; j < i; j++) {
                if (selected[j] == 0 && srflp_weight(i, j) >= aux) {
                    aux = srflp_weight(i, j);
                    *ps = i;
                    *pe = j;
                }
            }
        }

        selected[*ps] = selected[*pe] = 1;

        while (ps <= pe) {
            state->solution[utilized++] = *ps;
            ps++;
        }

        if (utilized == state->n - 1) {
            for (int i = 0; i < state->n; i++) {
                if (selected[i] == 0) {
                    state->solution[state->n - 1] = i;
                    selected[i] = 1;
                    utilized++;
                    break;
                }
            }
        }

    }

    FREE (selected);
    FREE (pp);
}

static void
initial_solution_deterministic (state *state) {
    for (int i = 0; i < state->n; i++) {
        state->solution[i] = i;
    }
}

static void
initial_solution_random (state *state) {
    int *numbers = NULL;
    ALLOC_N (numbers, state->n);

    for (int i = 0; i < state->n; i++)
        numbers[i] = i;

    int idx;
    int aux = state->n;

    for (int i = 0; i < state->n; i++) {
        idx = random_number (&seed) % aux;
        state->solution[i] = numbers[idx];
        if (idx < aux) {
            memmove (numbers + idx, (numbers + (idx + 1)), sizeof(int) * (aux - (idx + 1)));
        }
        aux -= 1;
     }

    FREE (numbers);
}

state *
state_init (initial_solution initial)
{
    state *state = NULL;

    if (ALLOC (state) == -1)
        FATAL_ERROR ("No hay memoria para reservar el objeto State.");

    state->n = srflp_problem_size ();
    state->solution = NULL;
    state->fitness = DBL_MAX;
    ALLOC_N (state->solution, state->n);

    switch (initial) {
        case DETERMINISTIC:
            initial_solution_deterministic (state);
            break;
        case GREEDY:
            initial_solution_greedy (state);
            break;
        case RANDOM:
            initial_solution_random (state);
        default:
            break;
    }

    return state;
}

state *
state_init_copy (state *other)
{
    state *new = state_init (NONE);
    new->fitness = other->fitness;
    memmove (new->solution, other->solution, sizeof(int) * other->n);
    return new;
}

double
state_fitness (state *state)
{
    return state->fitness;
}

void
state_set_fitness (state *state, double fitness)
{
    state->fitness = fitness;
}

void
state_set_solution (state *state, struct state *other)
{
    memmove(state->solution, other->solution, sizeof(int) * other->n);
}

state *
move_swap (state *s, int idx1, int idx2)
{
    state *new_state = state_init_copy (s);

    XORSWAP (new_state->solution[idx1], new_state->solution[idx2]);

    state_update_fitness (new_state);

    return new_state;
}

static state *
move_two_opt (state *st, int idx1, int idx2)
{
    state *new = state_init (NONE);

    int s = min(idx1, idx2);
    int e = max(idx1, idx2);

    memmove(new->solution, st->solution, sizeof(int) * s);

    int aux = 0;

    for (int i = s; i <= e; i++) {
        new->solution[i] = st->solution[e - aux];
        aux += 1;
    }

    memmove((new->solution + e + 1), (st->solution + e + 1), sizeof(int) * (st->n - (e + 1)));

    state_update_fitness (new);

    return new;
}

state *
state_random_neighbour (state *state, move_type move)
{
    int idx1, idx2;

    idx1 = idx2 = random_number (&seed) % state->n;

    while (idx1 == idx2)
        idx2 = random_number (&seed) % state->n;

    switch (move) {
        case SWAP:
            return move_swap (state, idx1, idx2);
        case TWO_OPT:
            return move_two_opt (state, idx1, idx2);
    }
}

void
state_update_fitness (state *state)
{
    double total = 0.0;
    double middle_distance;
    int p1, p2;

    state->fitness = 0.0;

    for (int i = 0; i < (state->n - 1); i++) {
        p1 = state->solution[i];
        middle_distance = 0.0;
        for (int j = (i + 1); j < state->n; j++) {
            p2 = state->solution[j];
            total += (srflp_facility_size(p1)/2.0 + middle_distance + srflp_facility_size(p2)/2.0) * srflp_weight(p1, p2);
            middle_distance += srflp_facility_size(p2);
        }
    }

    state->fitness = total;
}

void
state_print (const state *state, FILE *fp)
{
    printif(fp, "Solucion: \n");
    for (int i = 0; i < state->n; i++) {
        printif (fp, "%d ", state->solution[i]);
    }
    printif (fp, "\nCosto: %f\n", state->fitness);
}

void
state_free (state *state)
{
    if (state == NULL)
        return;
    FREE (state->solution);
    FREE (state);
}

int
state_equals (const state *s1, const state *s2) {
    for (int i = 0; i < s1->n; i++) {
        if (s1->solution[i] != s2->solution[i])
            return 0;
    }

    return 1;
}

