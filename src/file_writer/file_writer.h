#ifndef __FILE_WRITER__
#define __FILE_WRITER__

#include "../individual/individual.h"

/**
 * @brief Save the datas of the individual and cities in a JSON file located to the file path returned. 
 * According to the structure Individual and City, the data is written with the following format :
 *
 *{
 *  "cities": [
 *    {
 *      "insee_code": "59350",
 *      "name": "Lille",
 *      "region_code": "32",
 *      "region_name": "Hauts-de-France",
 *      "dept_code": "59",
 *      "dept_name": "Nord",
 *      "postal_code": "59000",
 *      "population": 236710,
 *      "latitude": 50.62920000,
 *      "longitude": 3.05730000
 *    },
 *    ...
 *  ],
 *  "individual":v{
 *      "chru_count": 50000,
 *      "isolated_population": 32510,
 *      "score": 278856,
 *      "hospitals": [
 *        {
 *          "chru_count": 1,
 *          "covered_population": 263831,
 *          "bed_count": 1500,
 *          "location": {
 *             "latitude": 50.62920000,
 *             "longitude": 3.05730000
 *            }
 *        },
 *        ...
 *      ]
 *  }
 *}
 *
 * @param filepath A pointer to the char array where the filepath string will be stored. Size has to be at least 100.
 * @param individual The pointer to the individual to save.
 * @param cities The pointer to the array of cities to save.
 * @param cities_size The size of the array cities.
 */
void save_individual_and_cities_in_JSON(char* filepath, Individual* individual, City* cities, int cities_size);


#endif