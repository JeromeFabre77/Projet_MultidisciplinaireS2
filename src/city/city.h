#ifndef __CITY__
#define __CITY__

/**
 * @struct City
 * @brief Represents a French municipality and its administrative data.
 */
typedef struct {
    char   insee_code[6];    /**< INSEE code (5 digits + '\0') */
    char   name[64];         /**< City name (Max real length: 46 + '\0' = 47 -> 64 for padding) */
    char   region_code[4];   /**< Region code (2 digits + '\0') -> Memory alignment */
    char   region_name[64];  /**< Region name (Matches city name buffer size) */
    char   dept_code[4];     /**< Department code */
    char   dept_name[64];    /**< Department name */
    char   postal_code[6];   /**< Postal code (5 digits + '\0') */
    int    population;       /**< Total population (Int fits up to ~2.1B) */
    double latitude;         /**< Geographic coordinates: Latitude */
    double longitude;        /**< Geographic coordinates: Longitude */
} City;

#endif