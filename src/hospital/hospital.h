#ifndef __GENETIC__
#define __GENETIC__

#include "../city/city.h"


/**
 * @struct Hospital
 * @brief Represents a hospital and the cities that it cover.
 */
typedef struct
{
    int is_chru;             /* 1 if located in a city > 80,000 */
    int covered_population;  /* Total population of all cities attached to this hospital */
    int bed_count;           /* Number beds of the hospital */
    const City *location;          /* The specific city where the hospital is built */
    const City **cities;           /* Array of cities attached to this hospital */
    int cities_size;         /* Size of the cities array */
} Hospital;


Hospital* create_new_hospital(const City *location);

void free_hospital(Hospital **hospital);

void add_covered_city_to_hospital(Hospital *hospital, const City *city);

void evaluate_hospital(Hospital *hospital);

#endif