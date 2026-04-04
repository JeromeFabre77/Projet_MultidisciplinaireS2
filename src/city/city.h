#ifndef __CITY__
#define __CITY__

#define INSEE_CODE_BYTES 5
#define CITY_NAME_BYTES 63
#define REGION_CODE_BYTES 3
#define REGION_NAME_BYTES 63
#define DEPT_CODE_BYTES 3
#define DEPT_NAME_BYTES 63
#define POSTAL_CODE_BYTES 5

/**
 * @struct City
 * @brief Represents a French municipality and its administrative data.
 */
typedef struct {
    char   insee_code[INSEE_CODE_BYTES + 1]; /**< INSEE code (5 digits + '\0') */
    char   name[CITY_NAME_BYTES + 1]; /**< City name */
    char   region_code[REGION_CODE_BYTES + 1]; /**< Region code */
    char   region_name[REGION_NAME_BYTES + 1]; /**< Region name */
    char   dept_code[DEPT_CODE_BYTES + 1]; /**< Department code */
    char   dept_name[DEPT_NAME_BYTES + 1]; /**< Department name */
    char   postal_code[POSTAL_CODE_BYTES + 1]; /**< Postal code */
    int    population;       /**< Total population (Int fits up to ~2.1B) */
    double latitude;         /**< Geographic coordinates: Latitude */
    double longitude;        /**< Geographic coordinates: Longitude */
} City;

void print_cities(const City *cities, int city_count, int count_to_print);

#endif