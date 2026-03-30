#ifndef __GENETIC__
#define __GENETIC__

#define COVERAGE_RADIUS_KM 10
#define CHRU_MIN_POPULATION 80000
#define BEDS_PER_1000 5.40

#include "../city/city.h"

/**
 * @struct Hospital
 * @brief Represents a medical facility and its coverage area.
 */
typedef struct {
    int is_chru;              /**< 1 if located in a city > 80,000 (+4000 in fitness) */
    int covered_population;  /**< Total population of all cities attached to this hospital */
    int bed_count;            /**< Number of available beds in the facility */
    City* location;            /**< The specific city where the hospital is built */
    City* cities;             /**< Array of cities served by this hospital */
    int cities_size;
} Hospital;

#endif