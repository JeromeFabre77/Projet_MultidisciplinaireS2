#ifndef __INDIVIDUAL__
#define __INDIVIDUAL__

#define PENALTY_HOSPITAL 5000 /* The penalty given for each hospital in the formula to compute the fitness */
#define BONUS_CHRU 4000 /* The bonus given for each CHRU in the formula to compute the fitness */

#include "../hospital/hospital.h"

/**
 * @struct Individual
 * @brief Represents a potential solution (a set of hospitals) in the genetic algorithm.
 */
typedef struct {
    int score;                  /* Fitness score (quality of the solution) */
    Hospital* hospitals;        /* Array of hospitals defining this solution */
    int hospitals_size;         /* Size of the array hospitals */
    int isolated_population;    /* Total population without hospital access */
    int total_population;       /* Total population */
    int chru_count;             /* Number of CHRU included */
} Individual;



void init_individual(Individual* individual);

int compare_individuals(const void* a, const void* b);

void create_random_individual(Individual* individual, City* cities, int cities_size);

void copy_individual(Individual* dest, const Individual* src, City *cities, int cities_size);

void cross_two_individuals(Individual *child, const Individual *first_parent, const Individual *second_parent, City *cities, int cities_size);

#endif