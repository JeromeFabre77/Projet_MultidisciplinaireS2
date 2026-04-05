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


#define INDIVIDUALS_SIZE 100 /* The number of individual for each generation */

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



    printf("Generate first génération ...\n");

    Generation first_gen;
    first_gen.number = 1;
    first_gen.individuals = malloc(INDIVIDUALS_SIZE * sizeof(Individual));
    if (first_gen.individuals == NULL) {
        fprintf(stderr, "Error: Failed to allocate population memory.\n");
        return 1;
    }

    int i;
    for (i = 0; i < INDIVIDUALS_SIZE; i++) {
        create_random_individual(&first_gen.individuals[i], cities, cities_size);
            
        printf("Individual %d created. Fitness: %d\n", i, first_gen.individuals[i].score);
    }

    qsort(first_gen.individuals, INDIVIDUALS_SIZE, sizeof(Individual), compare_individuals);

    /* Affichage du TOP 5 pour vérification */
    printf("\n--- TOP 5 INDIVIDUALS ---\n");
    for (i = 0; i < 5 && i < INDIVIDUALS_SIZE; i++) {
        print_individual(&(first_gen.individuals[i]));
    }




    /* Clean everything */
    printf("Cleaning up memory...\n");
    free_cities(cities, cities_size);
    

    return 0;
}