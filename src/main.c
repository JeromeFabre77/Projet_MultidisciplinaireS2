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
#include "constants.h"

int main(void)
{
    City *cities = NULL;
    int cities_size = 0;
    int i;
    double lat_min, lat_max, lon_min, lon_max;
    long total_population;

    srand(time(NULL));

    /* Check if settings are correct */
    if (NB_ELITE + NB_CROSSOVER + NB_MUTATION + NB_RANDOM != INDIVIDUALS_SIZE)
    {
        fprintf(stderr, "Error from main : The sum of defined NB is not equals to INDIVIDUALS_SIZE\n");
        return 1;
    }

    /* Retrieve cities and compute the neighbors */

    printf("Parsing CSV file for cities ...\n");
    cities_size = get_cities_from_csv(PATH_TO_CSV_FILE, &cities);
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

    /*<< Genetic algorithm */

    /* First generation */
    printf("Generate first génération ...\n");
    Generation *current_gen = create_new_generation(1, INDIVIDUALS_SIZE);
    check_allocation(current_gen, "main() -> failed to allocate current_gen");
    int nb_individuals_filled = 0;
    for (i = 0; i < current_gen->individuals_size; i++)
    {
        generate_random_individual(current_gen->individuals[nb_individuals_filled], cities, cities_size, total_population);
        nb_individuals_filled++;
    }
    sort_generation_individuals_by_fitness(current_gen);
    display(cities, cities_size, lat_min, lat_max, lon_min, lon_max,
            current_gen->individuals[0], current_gen->number, total_population);

    /* Other generations */
    int z;
    for (z = 1; z < NB_GENERATIONS; z++)
    {
        printf("Generate generation %d based on generation %d ---\n", current_gen->number + 1, current_gen->number);
        Generation *next_gen = create_new_generation(current_gen->number + 1, INDIVIDUALS_SIZE);
        check_allocation(next_gen, "main() -> failed to allocate next_gen");

        int nb_individuals_filled = 0;

        /* Copy the bests individuals from the current generation into the next generation */
        for (i = 0; i < NB_ELITE; i++)
        {
            copy_individual(next_gen->individuals[nb_individuals_filled], current_gen->individuals[i], cities, cities_size, total_population);
            nb_individuals_filled++;
        }

        /* Generate childs from two parents of the previous generation draw randomly in the NB_SELECTED bests */
        /*
        for (i = 0; i < NB_CROSSOVER; i++)
        {
            int parent1_idx = run_tournament(current_gen, 3);
            int parent2_idx = run_tournament(current_gen, 3);

            while (parent2_idx == parent1_idx)
            {
                parent2_idx = run_tournament(current_gen, 3);
            }

            cross_two_individuals(next_gen->individuals[nb_individuals_filled],
                                  current_gen->individuals[parent1_idx],
                                  current_gen->individuals[parent2_idx],
                                  cities, cities_size, total_population);
            nb_individuals_filled++;
        }*/

        for (i = 0; i < NB_CROSSOVER; i++)
        {
            int parent1_idx = run_tournament(current_gen, 3);
            int parent2_idx = run_tournament(current_gen, 3);
            int parent3_idx = run_tournament(current_gen, 3);
            int parent4_idx = run_tournament(current_gen, 3);
            int parent5_idx = run_tournament(current_gen, 3);
            int parent6_idx = run_tournament(current_gen, 3);

            /* Avoid same parents */
            while (parent2_idx == parent1_idx)
            {
                parent2_idx = run_tournament(current_gen, 3);
            }

            while (parent3_idx == parent1_idx || parent3_idx == parent2_idx)
            {
                parent3_idx = run_tournament(current_gen, 3);
            }

            while (parent4_idx == parent1_idx || parent4_idx == parent2_idx || parent4_idx == parent3_idx)
            {
                parent4_idx = run_tournament(current_gen, 3);
            }

            while (parent5_idx == parent1_idx || parent5_idx == parent2_idx || parent5_idx == parent3_idx || parent5_idx == parent4_idx)
            {
                parent5_idx = run_tournament(current_gen, 3);
            }

            while (parent6_idx == parent1_idx || parent6_idx == parent2_idx || parent6_idx == parent3_idx || parent6_idx == parent4_idx || parent6_idx == parent5_idx)
            {
                parent6_idx = run_tournament(current_gen, 3);
            }

            cross_six_individuals(next_gen->individuals[nb_individuals_filled],
                                  current_gen->individuals[parent1_idx],
                                  current_gen->individuals[parent2_idx],
                                  current_gen->individuals[parent3_idx],
                                  current_gen->individuals[parent4_idx],
                                  current_gen->individuals[parent5_idx],
                                  current_gen->individuals[parent6_idx],
                                  cities, cities_size, total_population);

            nb_individuals_filled++;
        }

        /* Generate mutations from a parents of the previous generation draw randomly in the NB_ELITE bests*/
        for (i = 0; i < NB_MUTATION; i++)
        {
            int parent_idx = rand() % NB_ELITE;
            mutate_individual(next_gen->individuals[nb_individuals_filled],
                              current_gen->individuals[parent_idx],
                              cities, cities_size, total_population);
            nb_individuals_filled++;
        }

        /* Generate new random individuals */
        for (i = 0; i < NB_RANDOM; i++)
        {
            generate_random_individual(next_gen->individuals[nb_individuals_filled], cities, cities_size, total_population);
            nb_individuals_filled++;
        }

        free_generation(&current_gen); /* Free the content of the previous generation */

        current_gen = next_gen;

        sort_generation_individuals_by_fitness(current_gen);

        /* Display best individual of the generation */
        display(cities, cities_size, lat_min, lat_max, lon_min, lon_max,
                current_gen->individuals[0], current_gen->number, total_population);
    }

    free_generation(&current_gen); /* Free the very last generation */

    /*>> Genetic algorithm */

    /* Clean everything */
    printf("Cleaning up memory...\n");
    free_cities(cities, cities_size);
    free_window();

    return 0;
}