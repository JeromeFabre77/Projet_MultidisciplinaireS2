#ifndef __GENERATION__
#define __GENERATION__

#include "../individual/individual.h"

/**
 * @brief Represents a collection of individuals at a specific step of the algorithm.
 */
typedef struct {
    Individual** individuals;      /* Array of pointers to individuals */
    int          individuals_size; /* Size of the array individuals */
    int          number;           /* Generation number */
} Generation;

Generation* create_new_generation(int number, int individuals_size);

void free_generation(Generation **gen);

void sort_generation_individuals_by_fitness(Generation *gen);

int run_tournament(const Generation *current_gen, const int tournament_size);

#endif