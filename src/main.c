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

/* Paramètres globaux */
#define NB_GENERATIONS 50
#define INDIVIDUALS_SIZE 100 /* The number of individual for each generation */

/* Stratégie de sélection (sur la base du tri) */
#define SELECTION_RATE 0.5 /* On ne pioche que dans les 50% meilleurs */

#define POOL_SIZE (int)(INDIVIDUALS_SIZE * SELECTION_RATE) /* Nombre d'individus "éligibles" pour être parents */

/* Répartition de la nouvelle génération (Somme = 100%) */
#define ELITE_RATE 0.10     /* 10% : Copie conforme des meilleurs */
#define CROSSOVER_RATE 0.50 /* 50% : Enfants de deux parents d'élite */
#define MUTATION_RATE 0.20  /* 20% : Versions modifiées de l'élite */
#define RANDOM_RATE 0.20    /* 20% : Nouveaux explorateurs aléatoires */

/* Calcul des indices pour ton tableau de 100 individus */
#define NB_ELITE (int)(INDIVIDUALS_SIZE * ELITE_RATE)
#define NB_CROSS (int)(INDIVIDUALS_SIZE * CROSSOVER_RATE)
#define NB_MUTAT (int)(INDIVIDUALS_SIZE * MUTATION_RATE)
#define NB_RND (int)(INDIVIDUALS_SIZE * RANDOM_RATE)

int main(void)
{
    if (ELITE_RATE + CROSSOVER_RATE + MUTATION_RATE + RANDOM_RATE != 1)
    {
        fprintf(stderr, "Error: The sum of defined RATES is not 1\n");
        return 1;
    }

    if (NB_ELITE + NB_CROSS + NB_MUTAT + NB_RND != INDIVIDUALS_SIZE)
    {
        fprintf(stderr, "Error: The sum of defined NB is not equals to INDIVIDUALS_SIZE\n");
        return 1;
    }

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
    if (first_gen.individuals == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate population memory.\n");
        return 1;
    }

    int i;
    for (i = 0; i < INDIVIDUALS_SIZE; i++)
    {
        create_random_individual(&first_gen.individuals[i], cities, cities_size);

        printf("Individual %d created. Fitness: %d\n", i, first_gen.individuals[i].score);
    }

    qsort(first_gen.individuals, INDIVIDUALS_SIZE, sizeof(Individual), compare_individuals);



    /* Clean everything */
    printf("Cleaning up memory...\n");
    free_cities(cities, cities_size);

    return 0;
}