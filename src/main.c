#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "interface/interface.h"
#include "parser/parser.h"
#include "city/city.h"
#include "individual/individual.h"
#include "generation/generation.h"
#include "utils/utils.h"

#define NB_GENERATIONS 50    /* The number of generation that the program will process */
#define INDIVIDUALS_SIZE 200 /* The number of individual for each generation */

#define SELECTION_RATE 0.5                                   /* The rate of best individuals kept from the previous generation to compute the next generation*/
#define NB_SELECTED (int)(INDIVIDUALS_SIZE * SELECTION_RATE) /* The number of best indiviuals kept from the previous generation to compute the next generation */

/* Sum of those params as to be 1 */
#define ELITE_RATE 0.10                               /* The rate of individuals of the next generation that are the best from the previous generation */
#define NB_ELITE (int)(INDIVIDUALS_SIZE * ELITE_RATE) /* The number of the individuals of the next generation that are the best from the previous generation */

#define CROSSOVER_RATE 0.50 /* The rate of individuals of the next generation that are crossover between two individuals of the previous generation (selected randomly in the NB_SELECTED bests individuals) */
#define NB_CROSSOVER (int)(INDIVIDUALS_SIZE * CROSSOVER_RATE)

#define MUTATION_RATE 0.20 /* The rate of individuals of the next generation that are mutation of an individual of the previous generation (selected randomly in the NB_SELECTED bests individuals) */
#define NB_MUTAT (int)(INDIVIDUALS_SIZE * MUTATION_RATE)

#define RANDOM_RATE 0.20 /* The rate of individuals of the next generation that are completly random and independant from the previous generation */
#define NB_RND (int)(INDIVIDUALS_SIZE * RANDOM_RATE)

int main(void)
{
    City      *cities = NULL;
    int        cities_size = 0;
    int        i;
    int        gen_idx;
    double     lat_min, lat_max, lon_min, lon_max;
    long       total_population;
    Generation current_gen;
    Generation next_gen;

    srand(time(NULL));

    /* Check if settings are correct */
    if (ELITE_RATE + CROSSOVER_RATE + MUTATION_RATE + RANDOM_RATE != 1)
    {
        fprintf(stderr, "Error from main : The sum of defined RATES is not 1\n");
        return 1;
    }

    if (NB_ELITE + NB_CROSSOVER + NB_MUTAT + NB_RND != INDIVIDUALS_SIZE)
    {
        fprintf(stderr, "Error from main : The sum of defined NB is not equals to INDIVIDUALS_SIZE\n");
        return 1;
    }

    /* Retrieve cities and compute the neighbors */

    printf("Parsing CSV file for cities ...\n");
    cities_size = get_cities_from_csv("../assets/data/communes-france-metrople-2025.csv", &cities);
    if (cities_size < 0 || cities == NULL)
    {
        fprintf(stderr, "Error from main : Failed to load cities from CSV\n");
        return 1;
    }

    printf("Computing neighbors for each city ...\n");
    compute_city_neighbors(cities, cities_size);

    /* Compute bounds for map projection */

    lat_min = cities[0].latitude;
    lat_max = cities[0].latitude;
    lon_min = cities[0].longitude;
    lon_max = cities[0].longitude;
    compute_bounds(cities, cities_size, &lat_min, &lat_max, &lon_min, &lon_max);

    total_population = 0;
    for (i = 0; i < cities_size; i++)
        total_population += cities[i].population;


    /* Create window */
    create_window();

    /* Genetic algorithm */

    printf("Generate first génération ...\n");
    current_gen.number = 1;
    current_gen.individuals = malloc(INDIVIDUALS_SIZE * sizeof(Individual));
    if (current_gen.individuals == NULL)
    {
        fprintf(stderr, "Error from main : Failed to allocate population memory.\n");
        return 1;
    }

    for (i = 0; i < INDIVIDUALS_SIZE; i++)
    {
        create_random_individual(&current_gen.individuals[i], cities, cities_size);
    }

    for (gen_idx = 1; gen_idx < NB_GENERATIONS; gen_idx++)
    {
        /* Sort the previous generation */
        qsort(current_gen.individuals, INDIVIDUALS_SIZE, sizeof(Individual), compare_individuals);
        printf("Best score for generation %d: %d\n", gen_idx, current_gen.individuals[0].score);

        /* Display best individual of the generation */
        display(cities, cities_size, lat_min, lat_max, lon_min, lon_max,
                &current_gen.individuals[0], gen_idx, total_population);

        printf("Generate generation %d based on generation %d ---\n", gen_idx + 1, gen_idx);
        next_gen.number = gen_idx + 1;
        next_gen.individuals = malloc(INDIVIDUALS_SIZE * sizeof(Individual));
        if (next_gen.individuals == NULL)
        {
            fprintf(stderr, "Error from main: Memory allocation failed for generation %d\n", next_gen.number);
            break;
        }

        int current_idx = 0;

        /* Copy the best individuals from the current generation into the next generation */
        for (i = 0; i < NB_ELITE; i++)
        {
            copy_individual(&next_gen.individuals[current_idx], &current_gen.individuals[i], cities, cities_size);
            current_idx++;
        }

        /* Generate child from two parents of the previous generation draw randomly in the NB_SELECTED bests */
        for (i = 0; i < NB_CROSSOVER; i++)
        {
            int parent1_idx = rand() % NB_SELECTED;
            int parent2_idx = rand() % NB_SELECTED;

            cross_two_individuals(&next_gen.individuals[current_idx],
                                  &current_gen.individuals[parent1_idx],
                                  &current_gen.individuals[parent2_idx],
                                  cities, cities_size);
            current_idx++;
        }

        for (i = 0; i < NB_MUTAT; i++)
        {
            int parent_idx = rand() % NB_SELECTED;
            mutate_individual(&next_gen.individuals[current_idx],
                              &current_gen.individuals[parent_idx],
                              cities, cities_size);
            current_idx++;
        }

        /* TODO : For the moment, complete the generation with randoms individuals but it should be mutation and coupling */
        while (current_idx < INDIVIDUALS_SIZE)
        {
            create_random_individual(&next_gen.individuals[current_idx], cities, cities_size);
            current_idx++;
        }

        /* Clean all allocations from the previous generation */
        for (i = 0; i < INDIVIDUALS_SIZE; i++)
        {
            /* TO DO : free every individual and their content */
            /* free_individual_internals(&current_gen.individuals[i]);*/
        }
        free(current_gen.individuals);

        current_gen = next_gen;
    }

    /* Clean everything */
    printf("Cleaning up memory...\n");
    free_cities(cities, cities_size);

    return 0;
}