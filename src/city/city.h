#ifndef __CITY__
#define __CITY__


#define COVERAGE_RADIUS_KM 10 /* Radius in Km of the area of cities covered by an hospital */
#define DELTA_LAT (COVERAGE_RADIUS_KM / 111.0) /* Latitude delta that correspond to the value of radius (111 Km ~= 1 latitude degree) */

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
    City *city;      /* Pointer to the neighbor city */
    double distance; /* The distance separating the neighbor from the city */
} Neighbor;

/**
 * @struct City
 * @brief Represents a French municipality and its administrative data.
 */
struct City
{
    char insee_code[INSEE_CODE_BYTES + 1];   /**< INSEE code */
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
    int neighbors_size;                       /**<Number of neighbors */
};

void compute_city_neighbors(City *cities, int city_count);
void free_cities(City *cities, int city_count);
City *city_resize_array(City *cities, int city_count);

#endif