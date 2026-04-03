#ifndef __CITY__
#define __CITY__

#include "../utils/utils.h"

#define COVERAGE_RADIUS_KM 10

typedef struct City City;

/**
 * @struct Neighbor
 * @brief Represents a neighbor of a city.
 */
typedef struct
{
    City *city;      /**< Pointer of a city */
    double distance; /**< The distance separating the neighbor from the city*/
} Neighbor;

/**
 * @struct City
 * @brief Represents a French municipality and its administrative data.
 */
struct City
{
    char insee_code[6];   /**< INSEE code (5 digits + '\0') */
    char name[64];        /**< City name (Max real length: 46 + '\0' = 47 -> 64 for padding) */
    char region_code[4];  /**< Region code (2 digits + '\0') -> Memory alignment */
    char region_name[64]; /**< Region name (Matches city name buffer size) */
    char dept_code[4];    /**< Department code */
    char dept_name[64];   /**< Department name */
    char postal_code[6];  /**< Postal code (5 digits + '\0') */
    int population;       /**< Total population (Int fits up to ~2.1B) */
    double latitude;      /**< Geographic coordinates: Latitude */
    double longitude;     /**< Geographic coordinates: Longitude */
    Neighbor *neighbors;  /**< The list of neighbors */
    int neighbor_size;    /**<Number of neighbors */
};

void compute_city_neighbors(City *cities, int city_count);
void free_cities(City *cities, int city_count);

#endif