#ifndef __UTILS__
#define __UTILS__

#include "../city/city.h"

double deg_to_rad(const double deg);

double compute_distance_between(const City *city1, const City *city2);

void check_allocation(const void *pointer, const char *error_message);

void check_parameter(const void *parameter, const char *error_message);

#endif