#ifndef __CITY__
#define __CITY__

#include "../utils/utils.h"

#define COVERAGE_RADIUS_KM 10
#define INSEE_CODE_BYTES 5
#define CITY_NAME_BYTES 63
#define REGION_CODE_BYTES 3
#define REGION_NAME_BYTES 63
#define DEPT_CODE_BYTES 3
#define DEPT_NAME_BYTES 63
#define POSTAL_CODE_BYTES 5

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
    char insee_code[INSEE_CODE_BYTES + 1];   /**< INSEE code (5 digits + '\0') */
    char name[CITY_NAME_BYTES + 1];          /**< City name */
    char region_code[REGION_CODE_BYTES + 1]; /**< Region code */
    char region_name[REGION_NAME_BYTES + 1]; /**< Region name */
    char dept_code[DEPT_CODE_BYTES + 1];     /**< Department code */
    char dept_name[DEPT_NAME_BYTES + 1];     /**< Department name */
    char postal_code[POSTAL_CODE_BYTES + 1]; /**< Postal code */
    int population;                          /**< Total population (Int fits up to ~2.1B) */
    double latitude;                         /**< Geographic coordinates: Latitude */
    double longitude;                        /**< Geographic coordinates: Longitude */
    Neighbor *neighbors;                     /**< The list of neighbors */
    int neighbor_size;                       /**<Number of neighbors */
};

void compute_city_neighbors(City *cities, int city_count);
void free_cities(City *cities, int city_count);
City *city_resize_array(City *cities, int city_count);
void print_cities(const City *cities, int city_count, int count_to_print);

#endif