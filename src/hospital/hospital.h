#ifndef __GENETIC__
#define __GENETIC__

#include "../city/city.h"


/**
 * @struct Hospital
 * @brief Represents a medical facility and its coverage area.
 */
typedef struct
{
    int is_chru;             /* 1 if located in a city > 80,000 (+4000 in fitness) */
    int covered_population;  /* Total population of all cities attached to this hospital */
    int bed_count;           /* Number of available beds in the facility */
    City *location;          /* The specific city where the hospital is built */
    City **cities;           /* Array of cities served by this hospital */
    int cities_size;
} Hospital;

void init_hospital(Hospital* hospital);

void create_random_list_of_hospital(Hospital** hospitals, int* hospitals_size, City* cities, int cities_size);

#endif