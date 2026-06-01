#ifndef __INDIVIDUAL__
#define __INDIVIDUAL__

#include "../hospital/hospital.h"

/**
 * @struct Individual
 * @brief Represents a potential solution (a set of hospitals) in the genetic algorithm.
 */
typedef struct
{
    int score;               /* Fitness score (quality of the solution) */
    Hospital **hospitals;    /* Array of hospitals defining this solution */
    int hospitals_size;      /* Size of the array hospitals */
    int isolated_population; /* Total population without hospital access */
    int covered_population;  /* Total population with an hospital access */
    int chru_count;          /* Number of CHRU in the hospitals list */
} Individual;

Individual *create_new_individual(void);

void free_individual(Individual **individual);

int compare_individuals(const void *a, const void *b);

void generate_random_individual(Individual *individual, const City *cities, const int cities_size, const int total_population);

void copy_individual(Individual *dest, const Individual *src, const City *cities, const int cities_size, const int total_population);

void cross_two_individuals(Individual *child, const Individual *first_parent, const Individual *second_parent, const City *cities, const int cities_size, const int total_population);

void mutate_individual(Individual *child, const Individual *parent, const City *cities, const int cities_size, const int total_population);

void print_individual(const Individual *individual);

void cross_six_individuals(Individual *child, const Individual *first_parent, const Individual *second_parent, const Individual *third_parent, const Individual *fourth_parent, const Individual *fifth_parent, const Individual *sixth_parent, const City *cities, const int cities_size, const int total_population);
#endif