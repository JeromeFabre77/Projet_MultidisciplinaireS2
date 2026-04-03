#include "city.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

#define EARTH_RADIUS_KM 6371.0

/**
 * @brief Calculate the distance between two cities in km.
 *
 * @param city1 pointer of first city.
 *
 * @param city2 pointer of second city.
 *
 * @return A radiant which is a double.
 */
double city_distance_km(const City *city1, const City *city2)
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
 * @brief Add neighbors in cities.
 *
 * @param cities list of cities.
 *
 * @param city_count number of cities.
 *
 * @return void.
 */
void compute_city_neighbors(City *cities, int city_count)
{
    if (cities == NULL || city_count <= 0)
        return;

    int i, j;

    for (i = 0; i < city_count; i++)
    {
        cities[i].neighbor_size = 0;
        cities[i].neighbors = NULL;
    }

    for (i = 0; i < city_count; i++)
    {
        for (j = i + 1; j < city_count; j++)
        {
            double distance = city_distance_km(&cities[i], &cities[j]);

            if (distance <= COVERAGE_RADIUS_KM)
            {
                cities[i].neighbor_size++;
                cities[j].neighbor_size++;
            }
        }
    }

    for (i = 0; i < city_count; i++)
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

    int *fill_count = calloc(city_count, sizeof(int));
    if (fill_count == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for fill_count\n");
        return;
    }

    for (i = 0; i < city_count; i++)
    {
        for (j = i + 1; j < city_count; j++)
        {
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
void print_cities(const City *cities, int city_count, int count_to_print)
{
    int i;
    int limit;

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

    limit = city_count;
    if (count_to_print > 0 && count_to_print < city_count)
    {
        limit = count_to_print;
    }

    printf("┌─────────┬──────────────────────────┬──────────────────┬────────────┬────────────┐\n");
    printf("│ INSEE   │ City Name                │ Region           │ Population │ Lat/Lng    │\n");
    printf("├─────────┼──────────────────────────┼──────────────────┼────────────┼────────────┤\n");

    for (i = 0; i < limit; i++)
    {
        printf("│ %-7s │ %-24s │ %-16s │ %10d │ %.1f/%.1f │\n",
               cities[i].insee_code,
               cities[i].name,
               cities[i].region_name,
               cities[i].population,
               cities[i].latitude,
               cities[i].longitude);
    }

    printf("└─────────┴──────────────────────────┴──────────────────┴────────────┴────────────┘\n");
    printf("Total: %d cities displayed\n", limit);
}

void print_neighbor(City city)
{
    printf("%s\n", city.name);
    print_cities(city.neighbors->city, city.neighbor_size, city.neighbor_size);
}