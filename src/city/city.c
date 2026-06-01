#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "city.h"

#include "../utils/utils.h"

/**
 * @brief Compute all the neighbors of all cities and store it in the neighbors array in each city.
 *
 * It is based on the COVERAGE_RADIUS_KM.
 *
 * @param cities The array of all cities
 * @param cities_size The size of the array of all cities
 */
void compute_city_neighbors(City *cities, int cities_size)
{
    if (cities == NULL)
    {
        fprintf(stderr, "Error from compute_city_neighbors : cities is null\n");
        return;
    }
    if (cities_size <= 0)
    {
        fprintf(stderr, "Error from compute_city_neighbors : cities_size is 0 or minus\n");
        return;
    }

    /* This initialisation is not supposed to be here, but ok for the moment*/
    int i;
    for (i = 0; i < cities_size; i++)
    {
        cities[i].neighbors_size = 0;
        cities[i].neighbors = NULL;
    }

    /* Get the size of the neighbors array for each city for allocate it later */
    int j;
    for (i = 0; i < cities_size; i++)
    {
        /* For eject fast city to far away (more that one lat or lon) */
        double cos_lat1 = cos(deg_to_rad(cities[i].latitude));
        double delta_lon = (cos_lat1 > 0.001) ? (DELTA_LAT / cos_lat1) : 360.0;

        /* j= i + 1 to not compute twice the same pair */
        for (j = i + 1; j < cities_size; j++)
        {
            /* For eject fast city to far away (more that one lat or lon) */
            if (fabs(cities[j].latitude - cities[i].latitude) > DELTA_LAT)
                continue;
            if (fabs(cities[j].longitude - cities[i].longitude) > delta_lon)
                continue;

            double distance = compute_distance_between(&cities[i], &cities[j]);

            if (distance <= COVERAGE_RADIUS_KM)
            {
                cities[i].neighbors_size++;
                cities[j].neighbors_size++;
            }
        }
    }

    /* Allocate the neighbors array for each city */
    for (i = 0; i < cities_size; i++)
    {
        if (cities[i].neighbors_size > 0)
        {
            cities[i].neighbors = malloc(cities[i].neighbors_size * sizeof(Neighbor));
            if (cities[i].neighbors == NULL)
            {
                fprintf(stderr, "Error compute_city_neighbors : Memory allocation failed\n");
                return;
            }
        }
    }

    /* Fill the neighbors array for each city */
    int *fill_count = calloc(cities_size, sizeof(int));
    if (fill_count == NULL)
    {
        fprintf(stderr, "Error from compute_city_neighbors : Memory allocation failed\n");
        return;
    }

    for (i = 0; i < cities_size; i++)
    {
        /* For eject fast city to far away (more that one lat or lon) */
        double cos_lat1 = cos(deg_to_rad(cities[i].latitude));
        double delta_lon = (cos_lat1 > 0.001) ? (DELTA_LAT / cos_lat1) : 360.0;

        /* j= i + 1 to not compute twice the same pair */
        for (j = i + 1; j < cities_size; j++)
        {
            /* For eject fast city to far away (more that one lat or lon) */
            if (fabs(cities[j].latitude - cities[i].latitude) > DELTA_LAT)
                continue;
            if (fabs(cities[j].longitude - cities[i].longitude) > delta_lon)
                continue;

            double distance = compute_distance_between(&cities[i], &cities[j]);

            if (distance <= COVERAGE_RADIUS_KM)
            {
                int pos_i = fill_count[i];
                int pos_j = fill_count[j];

                cities[i].neighbors[pos_i].city = &cities[j];
                cities[i].neighbors[pos_i].distance = distance;
                fill_count[i]++;

                cities[j].neighbors[pos_j].city = &cities[i];
                cities[j].neighbors[pos_j].distance = distance;
                fill_count[j]++;
            }
        }
    }

    free(fill_count);
}

/**
 * @brief free a list of cities.
 *
 * @param cities The array of all cities
 * @param cities_size The size of the array of all cities
 *
 * @return void.
 */
void free_cities(City *cities, int city_size)
{
    if (cities == NULL)
        return;

    int i;
    for (i = 0; i < city_size; i++)
    {
        free(cities[i].neighbors);
        cities[i].neighbors = NULL;
        cities[i].neighbors_size = 0;
    }

    free(cities);
}

/**
 * @brief Alloue ou reajuste dynamiquement un tableau de City.
 * @param cities Pointeur courant du tableau (NULL pour une premiere allocation).
 * @param city_count Nombre d'elements City souhaites.
 * @return Nouveau pointeur valide, ou NULL en cas d'echec (ou si city_count <= 0).
 */
City *city_resize_array(City *cities, int city_size)
{
    City *resized;

    if (city_size <= 0)
    {
        free(cities);
        return NULL;
    }

    resized = (City *)realloc(cities, city_size * sizeof(City));
    if (resized == NULL)
    {
        fprintf(stderr, "Error: Memory reallocation failed for cities array\n");
        return NULL;
    }

    return resized;
}