#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "interface/interface.h"
#include "parser/parser.h"
#include "city/city.h"
#include "hospital/hospital.h"
#include "utils/utils.h"

int main(void)
{
    City *cities = NULL;
    int cities_size = 0;

    printf("Parsing CSV file for cities ...\n");
    cities_size = get_cities_from_csv("../assets/data/communes-france-metrople-2025.csv", &cities);
    if (cities_size < 0 || cities == NULL)
    {
        fprintf(stderr, "Error: Failed to load cities from CSV\n");
        return 1;
    }

    print_cities(cities, cities_size, 10);

    printf("Computing neighbors for each city ...\n");
    compute_city_neighbors(cities, cities_size);

    print_neighbor(cities[0]);

    Hospital* hospitals = NULL;
    int hospitals_size = 0;
    int err_code = create_random_list_of_hospital(&hospitals, &hospitals_size, cities, cities_size);
    if(err_code != 0) {
        fprintf(stderr, "Error: Failed to create a random list of hospital\n");
        return 1;
    }

    print_hospitals(hospitals, hospitals_size);

    if (hospitals != NULL) {
        free(hospitals);
    }

    free_cities(cities, cities_size);
    

    return 0;
}