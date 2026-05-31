#ifndef __GENERATION__
#define __GENERATION__

#include "../individual/individual.h"

/**
 * @brief Represents a collection of individuals at a specific step of the algorithm.
 */
typedef struct {
    Individual* individuals; /* Array of individuals forming the population */
    int number;              /* Generation number */
} Generation;

#endif