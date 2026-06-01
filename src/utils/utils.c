#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "utils.h"

#include "../constants.h"


/**
 * @brief Convert degres to radiants.
 * @param deg is a double.
 * @return A radiant which is a double.
 */
double deg_to_rad(const double deg)
{
    return deg * M_PI / 180.0;
}

/**
 * @brief Calculate the distance between two cities in Km.
 * @param city1 pointer of first city.
 * @param city2 pointer of second city.
 * @return A double that is the distance in Km between the two cities.
 */
double compute_distance_between(const City *city1, const City *city2)
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
 * @brief Check if a memory allocation was successful.
 * If the pointer is NULL, prints an error message and terminates the program.
 * @param pointer The pointer to check (typically returned by malloc/calloc).
 * @param error_message The context or details to display in case of failure.
 */
void check_allocation(const void *pointer, const char *error_message)
{
    if (pointer == NULL)
    {
        fprintf(stderr, "\n--- FATAL ERROR: Memory Allocation Failed ---\n");
        fprintf(stderr, "Context: %s\n", error_message);
        fprintf(stderr, "The program will now exit.\n---------------------------------------------\n");
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Check if a required function parameter is NULL.
 * If the parameter is NULL, prints an error message and terminates the program.
 * @param parameter The pointer/parameter to validate.
 * @param error_message The context or details to display in case of failure.
 */
void check_parameter(const void *parameter, const char *error_message)
{
    if (parameter == NULL)
    {
        fprintf(stderr, "\n--- FATAL ERROR: Invalid Parameter (NULL) ---\n");
        fprintf(stderr, "Context: %s\n", error_message);
        fprintf(stderr, "The program will now exit.\n---------------------------------------------\n");
        exit(EXIT_FAILURE);
    }
}
