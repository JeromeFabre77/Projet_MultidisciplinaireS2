#include "hospital.h"

#include <stdio.h>
#include <stdlib.h>

#include "../constants.h"
#include "../utils/utils.h"

/**
 * @brief Allocates and initializes a new hospital at a given city location.
 * @param location A pointer to the constant city where the hospital is placed.
 * @return A pointer to the newly created hospital.
 */
Hospital *create_new_hospital(const City *location)
{
    check_parameter(location, "create_new_hospital() -> parameter 'location' cannot be NULL");

    Hospital *hospital = malloc(sizeof(Hospital));
    check_allocation(hospital, "create_new_hospital() -> allocating Hospital struct");

    hospital->location = location;
    if (location->population >= CHRU_MIN_POPULATION)
    {
        hospital->is_chru = 1;
    }
    else
    {
        hospital->is_chru = 0;
    }

    hospital->covered_population = 0;
    hospital->bed_count = 0;
    hospital->cities = NULL;
    hospital->cities_size = 0;

    return hospital;
}

/**
 * @brief Safely frees a hospital structure and its associated resources.
 * @param hospital A double pointer to the hospital to be freed, which will be set to NULL.
 */
void free_hospital(Hospital **hospital)
{
    if (hospital == NULL || *hospital == NULL)
    {
        return;
    }

    if ((*hospital)->cities != NULL)
    {
        free((*hospital)->cities);
        (*hospital)->cities = NULL;
    }

    free(*hospital);
    *hospital = NULL;
}

/**
 * @brief Calculates the total population covered by a given hospital.
 * @param hospital A pointer to the constant hospital to analyze.
 * @return The sum of the populations of all cities covered by the hospital.
 */
int count_covered_population_for_hospital(const Hospital *hospital)
{
    check_parameter(hospital, "count_covered_population_for_hospital() -> parameter 'hospital' is null");

    int total_covered_population = 0;

    int i;
    for (i = 0; i < hospital->cities_size; i++)
    {
        total_covered_population += hospital->cities[i]->population;
    }

    return total_covered_population;
}

/**
 * @brief Calculates the total number of beds required for a given hospital based on its covered population.
 * @param hospital A pointer to the constant hospital to analyze.
 * @return The total number of required beds.
 */
int count_bed_count_for_hospital(const Hospital *hospital)
{
    check_parameter(hospital, "count_bed_count_for_hospital() -> parameter 'hospital' is null");

    return ((double)(hospital->covered_population) / 1000) * BED_FOR_POPULATION;
}

/**
 * @brief Adds a city to the list of cities covered by a given hospital.
 * @param hospital A pointer to the hospital structure to modify.
 * @param city A pointer to the constant city being added to the coverage list.
 */
void add_covered_city_to_hospital(Hospital *hospital, const City *city)
{
    check_parameter(hospital, "add_covered_city_to_hospital() -> parameter 'hospital' is null");
    check_parameter(city, "add_covered_city_to_hospital() -> parameter 'city' is null");

    int new_size = hospital->cities_size + 1;

    hospital->cities = realloc(hospital->cities, new_size * sizeof(City *));
    check_allocation(hospital->cities, "add_covered_city_to_hospital() -> allocating hospital->cities");

    hospital->cities[hospital->cities_size] = city;

    hospital->cities_size = new_size;
}

/**
 * @brief Evaluates a hospital by updating its total covered population and required bed count.
 * @param hospital A pointer to the hospital structure to evaluate and update.
 */
void evaluate_hospital(Hospital *hospital)
{
    check_parameter(hospital, "evaluate_hospital() -> parameter 'hospital' is null");

    hospital->covered_population = count_covered_population_for_hospital(hospital);
    hospital->bed_count = count_bed_count_for_hospital(hospital);
}