#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "utils.h"

#define M_PI 3.1415
#define EARTH_RADIUS_KM 6371.0

/**
 * @brief Convert degres to radiants.
 * @param deg is a double.
 * @return A radiant which is a double.
 */
double deg_to_rad(double deg)
{
    return deg * M_PI / 180.0;
}

/**
 * @brief Calculate the distance between two cities in Km.
 * @param city1 pointer of first city.
 * @param city2 pointer of second city.
 * @return A double that is the distance in Km between the two cities.
 */
double get_distance_between(City *city1, City *city2)
{
    double lat1 = deg_to_rad(city1->latitude);
    double lon1 = deg_to_rad(city1->longitude);
    double lat2 = deg_to_rad(city2->latitude);
    double lon2 = deg_to_rad(city2->longitude);

    double dlat = lat2 - lat1;
    double dlon = lon2 - lon1;

    double a = sin(dlat / 2.0) * sin(dlat / 2.0) + cos(lat1) * cos(lat2) * sin(dlon / 2.0) * sin(dlon / 2.0);

    if (a > 1.0)
        a = 1.0;
    if (a < 0.0)
        a = 0.0;

    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return EARTH_RADIUS_KM * c;
}

/**
 * @brief Dynamically allocates or resizes a City array.
 * @param cities Current array pointer (NULL for the initial allocation).
 * @param city_count Desired number of City elements.
 * @return A valid new pointer, or NULL on failure (or if city_count <= 0).
 */
City *city_resize_array(City *cities, int city_count) {
	City *resized;

	/* If the requested size is zero (or negative),
	 * free the existing array and indicate that there is no more data. */
	if (city_count <= 0) {
		free(cities);
		return NULL;
	}

	/* realloc handles both cases:
	 * - cities == NULL: behavior equivalent to malloc
	 * - cities != NULL: resize the existing block */
	resized = (City *)realloc(cities, city_count * sizeof(City));
	if (resized == NULL) {
		fprintf(stderr, "Error: Memory reallocation failed for cities array\n");
		return NULL;
	}

	return resized;
}