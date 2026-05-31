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

