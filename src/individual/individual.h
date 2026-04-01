#ifndef __INDIVIDUAL__
#define __INDIVIDUAL__

#define HOSPITAL_COST 5000
#define CHRU_BONUS 4000
#define BORNE_MIN 10
#define BORNE_MAX 20

#include "../genetic/genetic.h"

/**
 * @struct Individual
 * @brief Represents a potential solution (a set of hospitals) in the genetic algorithm.
 */
typedef struct {
    int score;                  /**< Fitness score (quality of the solution) */
    Hospital* hospitals;        /**< Array of hospitals defining this solution */
    int hospitals_size;
    int isolated_population;   /**< Total population without hospital access */
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

#endif