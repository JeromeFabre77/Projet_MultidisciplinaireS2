#include "city.h"
#include <stdlib.h>
#include <math.h>

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
    if (cities == NULL || city_count == 0)
        return;
    int i;
    for (i = 0; i < city_count; i++)
    {
        cities[i].neighbors = NULL;
        cities[i].neighbor_size = 0;
    }

    for (i = 0; i < city_count; i++)
    {
        int j;
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
            cities[i].neighbors = malloc((int)cities[i].neighbor_size * sizeof(Neighbor));
            if (cities[i].neighbors == NULL)
            {
                int k;
                for (k = 0; k < i; k++)
                {
                    free(cities[k].neighbors);
                    cities[k].neighbors = NULL;
                    cities[k].neighbor_size = 0;
                }
                return;
            }
        }
    }

    int *fill_count = (int *)calloc(city_count, sizeof(int));
    if (fill_count == NULL)
    {
        for (i = 0; i < city_count; i++)
        {
            free(cities[i].neighbors);
            cities[i].neighbors = NULL;
            cities[i].neighbor_size = 0;
        }
        return;
    }

    for (i = 0; i < city_count; i++)
    {
        int j;
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