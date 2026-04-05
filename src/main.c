#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "interface/interface.h"
#include "parser/parser.h"
#include "city/city.h"
#include "hospital/hospital.h"
#include "individual/individual.h"
#include "utils/utils.h"

int main(void)
{
    srand(time(NULL));

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

    printf("\nGenerating a random individual...\n");
    Individual* my_first_solution = create_random_individual(cities, cities_size);

    if (my_first_solution == NULL) {
        fprintf(stderr, "Error: Failed to create individual.\n");
        free_cities(cities, cities_size);
        return 1;
    }
    print_individual(my_first_solution, cities_size);

    printf("Evaluating individual...\n");
    evaluate_individual(my_first_solution, cities, cities_size);
    print_individual(my_first_solution, cities_size);





    /* Clean everything */
    printf("Cleaning up memory...\n");
    free_cities(cities, cities_size);
    

    return 0;
}