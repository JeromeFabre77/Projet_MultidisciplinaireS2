#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#include "city.h"

#include "../utils/utils.h"

/**
 * @brief Add neighbors in cities.
 *
 * @param cities list of cities.
 *
 * @param cities_size number of cities.
 *
 * @return void.
 */
void compute_city_neighbors(City *cities, int cities_size)
{
    if (cities == NULL)
    {
        fprintf(stderr, "Error: cities is null\n");
        return;
    }
    if (cities_size <= 0)
    {
        fprintf(stderr, "Error: cities_size is 0 or minus\n");
        return;
    }

    /* This initialisation is not supposed to be here, but ok for the moment*/
    int i;
    for (i = 0; i < cities_size; i++)
    {
        cities[i].neighbor_size = 0;
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

            double distance = city_distance_km(&cities[i], &cities[j]);

            if (distance <= COVERAGE_RADIUS_KM)
            {
                cities[i].neighbor_size++;
                cities[j].neighbor_size++;
            }
        }
    }

    /* Allocate the neighbors array for each city */
    for (i = 0; i < cities_size; i++)
    {
        if (cities[i].neighbor_size > 0)
        {
            cities[i].neighbors = malloc(cities[i].neighbor_size * sizeof(Neighbor));
            if (cities[i].neighbors == NULL)
            {
                fprintf(stderr, "Error: Memory allocation failed for neighbors array\n");
                return;
            }
        }
    }

    /* Fill the neighbors array for each city */
    int *fill_count = calloc(cities_size, sizeof(int));
    if (fill_count == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for fill_count\n");
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

            double distance = city_distance_km(&cities[i], &cities[j]);

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
 * @param cities list of cities.
 *
 * @param city_count number of cities.
 *
 * @return void.
 */
void free_cities(City *cities, int city_count)
{
    if (cities == NULL)
        return;

    int i;
    for (i = 0; i < city_count; i++)
    {
        free(cities[i].neighbors);
        cities[i].neighbors = NULL;
        cities[i].neighbor_size = 0;
    }

    free(cities);
}

/**
 * @brief Alloue ou reajuste dynamiquement un tableau de City.
 * @param cities Pointeur courant du tableau (NULL pour une premiere allocation).
 * @param city_count Nombre d'elements City souhaites.
 * @return Nouveau pointeur valide, ou NULL en cas d'echec (ou si city_count <= 0).
 */
City *city_resize_array(City *cities, int city_count)
{
    City *resized;

    if (city_count <= 0)
    {
        free(cities);
        return NULL;
    }

    resized = (City *)realloc(cities, city_count * sizeof(City));
    if (resized == NULL)
    {
        fprintf(stderr, "Error: Memory reallocation failed for cities array\n");
        return NULL;
    }

    return resized;
}

/* Temporary function kept for debugging/preview until UI rendering fully replaces terminal output. */
void print_cities(City *cities, int city_count, int count_to_print)
{
    if (cities == NULL)
    {
        fprintf(stderr, "Error: cities pointer is NULL\n");
        return;
    }

    if (city_count < 0)
    {
        fprintf(stderr, "Error: city_count is invalid\n");
        return;
    }

    if (count_to_print < 0)
    {
        fprintf(stderr, "Error: count_to_print is invalid\n");
        return;
    }

    if (count_to_print > city_count)
    {
        fprintf(stderr, "Error: count_to_print is superior to city_count\n");
        return;
    }

    int i;
    for (i = 0; i < count_to_print; i++)
    {
        printf("%s, %s, %s, %d, %f/%f\n",
               cities[i].insee_code,
               cities[i].name,
               cities[i].region_name,
               cities[i].population,
               cities[i].latitude,
               cities[i].longitude);
    }
    printf("End: %d cities displayed on %d\n", count_to_print, city_count);
}

/* Temporary function kept for debugging/preview until UI rendering fully replaces terminal output. */
void print_neighbor(City city)
{
    printf("\nNeighbors for city %s (%d neighbors found):\n", city.name, city.neighbor_size);

    if (city.neighbors == NULL || city.neighbor_size == 0)
    {
        printf("  (No neighbors found within %d km)\n", COVERAGE_RADIUS_KM);
        return;
    }

    int i;
    for (i = 0; i < city.neighbor_size; i++)
    {
        City *nb = city.neighbors[i].city;
        double dist = city.neighbors[i].distance;

        printf("%s (Dist: %.2f km)\n", nb->name, dist);
    }
    printf("End of neighbors list.\n");
}