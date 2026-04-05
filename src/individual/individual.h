#ifndef __INDIVIDUAL__
#define __INDIVIDUAL__

#define PENALTY_HOSPITAL 5000
#define BONUS_CHRU 4000

#include "../hospital/hospital.h"

/**
 * @struct Individual
 * @brief Represents a potential solution (a set of hospitals) in the genetic algorithm.
 */
typedef struct {
    int score;                  /**< Fitness score (quality of the solution) */
    Hospital* hospitals;        /**< Array of hospitals defining this solution */
    int hospitals_size;
    int isolated_population;   /**< Total population without hospital access */
    int total_population;
    int chru_count;             /**< Number of CHRU included */
} Individual;

/**
 * @struct Generation
 * @brief Represents a collection of individuals at a specific step of the algorithm.
 */
typedef struct {
    Individual* individuals; /**< Array of individuals forming the population */
    int number;              /**< Generation number */
} Generation;


void create_random_individual(Individual* individual, City* cities, int cities_size);
compare_individuals(const void* a, const void* b);
void print_individual(const Individual* ind);

#endif