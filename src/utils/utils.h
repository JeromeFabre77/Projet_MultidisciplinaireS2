#ifndef __UTILS__
#define __UTILS__

#include "../city/city.h"

double deg_to_rad(double deg);

double get_distance_between(City *city1, City *city2);

City *city_resize_array(City *cities, int city_count);

#endif