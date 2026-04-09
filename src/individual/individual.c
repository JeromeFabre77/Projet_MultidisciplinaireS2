#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "individual.h"

#define CHRU_MIN_POPULATION 80000
#define BED_FOR_POPULATION 5.4 /* Number of bed needed for 1000 people covered by an hospital*/

/**
 * @brief Initialize an individual with null or 0 values. It MUST be use after a memory allocation for create an individual.
 * @param individual The pointer to individual to initialize
 */
void init_individual(Individual *individual)
{
    if (individual == NULL)
    {
        fprintf(stderr, "Error from init_individual: individual is null\n");
        return;
    }

    individual->score = 0;
    individual->hospitals = NULL;
    individual->hospitals_size = 0;
    individual->chru_count = 0;
    individual->isolated_population = 0;
    individual->total_population = 0;
}

/**
 * @brief Compare to induviduals depending on their score. Usefull for using qsort
 * @param a the first individual
 * @param b the second individual
 * @return 0 if they are equals, 1 if the b is higher that a, -1 if a is higher thant b
 */
int compare_individuals(const void *a, const void *b)
{
    const Individual *indA = (const Individual *)a;
    const Individual *indB = (const Individual *)b;

    if (indB->score > indA->score)
    {
        return 1;
    }
    else if (indB->score < indA->score)
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief Compute all stats of an individual including :
 *  - the cities impacted by each hospital of the array hospitals of the individual
 *  - the value is_chru of each hospital
 *  - the covered population of each hospital
 *  - the bed count of each hospital
 *  - the number of chru of the individual
 *  - the population isolated
 *  - the total population
 *  - the score of the individual
 *
 * @param individual A pointer to the individual to evaluate
 * @param cities The array of all cities
 * @param cities_size The size of the array of all cities
 */
void evaluate_individual(Individual *individual, City *cities, int cities_size)
{
    if (individual == NULL)
    {
        fprintf(stderr, "Error from evaluate_individual: individual is null\n");
        return;
    }
    if (cities == NULL)
    {
        fprintf(stderr, "Error from evaluate_individual : cities is null\n");
        return;
    }
    if (cities_size <= 0)
    {
        fprintf(stderr, "Error from evaluate_individual : cities_size is 0\n");
        return;
    }

    /* Temporary variables to get a fast access to informations */
    int *hospital_at_city = malloc(cities_size * sizeof(int));                           /* Is an hospital present in a city, -1 if no, the index of the hospital if yes*/
    int *city_to_nearest_hospital = malloc(cities_size * sizeof(int));                   /* What is the nearest hospital from a city, -1 if no hospital, the index of the hospital nearest from the city */
    int *nb_city_attached_to_hospital = calloc(individual->hospitals_size, sizeof(int)); /* Number of cities attached to each hospital */
    if (!hospital_at_city || !city_to_nearest_hospital || !nb_city_attached_to_hospital)
    {
        fprintf(stderr, "Error from evaluate_individual : Memory allocation failed\n");
        return;
    }

    int i;
    for (i = 0; i < cities_size; i++)
    {
        hospital_at_city[i] = -1;
        city_to_nearest_hospital[i] = -1;
    }

    /* Fill the hospital_at_city variable and count the number of CHRU*/
    for (i = 0; i < individual->hospitals_size; i++)
    {
        int idx = (int)(individual->hospitals[i].location - cities);
        hospital_at_city[idx] = i;

        if (individual->hospitals[i].location->population >= CHRU_MIN_POPULATION)
        {
            individual->hospitals[i].is_chru = 1;
        }

        if (individual->hospitals[i].is_chru)
        {
            individual->chru_count++;
        }
    }

    /* Fill the city_to_nearest_hospital variable */
    for (i = 0; i < cities_size; i++)
    {
        int nearest_hospital_idx = -1;

        /* If there is an hospital in the city, this hospital is the nearest for sure */
        if (hospital_at_city[i] != -1)
        {
            nearest_hospital_idx = hospital_at_city[i];
        }
        else
        {
            float min_distance = 1e9;

            /* For each neighbor of the city, check if there is an hospital in it, if no the city is isolated,
            if yes find the one with the minimal distance*/
            int j;
            for (j = 0; j < cities[i].neighbors_size; j++)
            {
                int neighbor_idx = (int)(cities[i].neighbors[j].city - cities);

                if (hospital_at_city[neighbor_idx] != -1)
                {
                    if (cities[i].neighbors[j].distance < min_distance)
                    {
                        min_distance = cities[i].neighbors[j].distance;
                        nearest_hospital_idx = hospital_at_city[neighbor_idx];
                    }
                }
            }
        }

        city_to_nearest_hospital[i] = nearest_hospital_idx;

        if (nearest_hospital_idx != -1)
        {
            nb_city_attached_to_hospital[nearest_hospital_idx]++;
        }
    }

    /* Allocate the memory of cities array for each hospital */
    for (i = 0; i < individual->hospitals_size; i++)
    {
        if (nb_city_attached_to_hospital[i] > 0)
        {
            individual->hospitals[i].cities = malloc(nb_city_attached_to_hospital[i] * sizeof(City *));
            individual->hospitals[i].cities_size = 0; /* Set to 0 for the moment to use it as a index will filling */
        }
    }

    /* Fill the cities array of each hospital by crossing each cities and check which hospital is the nearest */
    int total_covered_pop = 0;
    for (i = 0; i < cities_size; i++)
    {
        int hospital_idx = city_to_nearest_hospital[i];

        if (hospital_idx != -1)
        {
            individual->hospitals[hospital_idx].cities[individual->hospitals[hospital_idx].cities_size] = &cities[i];

            individual->hospitals[hospital_idx].covered_population += cities[i].population;

            individual->hospitals[hospital_idx].cities_size++;

            total_covered_pop += cities[i].population;
        }
        else
        {
            individual->isolated_population += cities[i].population;
        }
    }

    /* Evaluate last statistics not already done */
    individual->total_population = total_covered_pop;
    individual->score = individual->total_population - individual->isolated_population - (PENALTY_HOSPITAL * individual->hospitals_size) + (BONUS_CHRU * individual->chru_count);

    for (i = 0; i < individual->hospitals_size; i++)
    {
        individual->hospitals[i].bed_count = (individual->hospitals[i].covered_population / 1000) * BED_FOR_POPULATION;
    }

    /* Free temporary variables */
    free(hospital_at_city);
    free(city_to_nearest_hospital);
    free(nb_city_attached_to_hospital);
}

/**
 * @brief Takes an empty individual and fill it with a random list of hospital and then evaluate it
 * @param individual A pointer to the individual
 * @param cities The array of all cities
 * @param cities_size The size of the array of all cities
 */
void create_random_individual(Individual *individual, City *cities, int cities_size)
{
    if (individual == NULL)
    {
        fprintf(stderr, "Error from create_random_individual : individual is null\n");
        return;
    }

    init_individual(individual);

    create_random_list_of_hospital(&(individual->hospitals), &individual->hospitals_size, cities, cities_size);

    evaluate_individual(individual, cities, cities_size);
}

/**
 * @brief Takes an empty individual and another individual and copy the list of hospital of the second in the first, then
 * evaluate it.
 * @param individual A pointer to the individual
 * @param cities The array of all cities
 * @param cities_size The size of the array of all cities
 */
void copy_individual(Individual *dest, const Individual *src, City *cities, int cities_size)
{
    if (dest == NULL)
    {
        fprintf(stderr, "Error from copy_individual : dest is null\n");
        return;
    }
    if (src == NULL)
    {
        fprintf(stderr, "Error from copy_individual : src is null\n");
        return;
    }

    init_individual(dest);

    dest->hospitals_size = src->hospitals_size;
    dest->hospitals = malloc(dest->hospitals_size * sizeof(Hospital));
    if (dest->hospitals == NULL)
    {
        fprintf(stderr, "Error from copy_individual : Memory allocation failed\n");
        return;
    }

    int i;
    for (i = 0; i < dest->hospitals_size; i++)
    {
        init_hospital(&dest->hospitals[i]);

        dest->hospitals[i].location = src->hospitals[i].location;
        dest->hospitals[i].is_chru = src->hospitals[i].is_chru;
    }

    evaluate_individual(dest, cities, cities_size);
}

/**
 * @brief Cross two individuals and return the crossed individual.
 *
 * Takes into consideration :
 * - The different size of the list, the result will be 50% of the first list and 50% of the second list.
 * - The fact that hospitals are ordered like the city list, so the result have to contain as much elements from the beginning of lists as element from the end of lists.
 * - TO DO : For the moment it do not handle the doublons, so the child can have 2 hospitals in the same city.
 *
 * @param first_parent    A pointer to the first individual you want to cross
 * @param second_parent    A pointer to the second individual you want to cross
 * @return The individual generated by crossing the two individuals given in parameters.
 */
void cross_two_individuals(Individual *child, const Individual *first_parent, const Individual *second_parent, City *cities, int cities_size)
{
    if (child == NULL)
    {
        fprintf(stderr, "Error from cross_two_individuals : child is null\n");
        return;
    }
    if (first_parent == NULL)
    {
        fprintf(stderr, "Error from cross_two_individuals : first_parent is null\n");
        return;
    }
    if (second_parent == NULL)
    {
        fprintf(stderr, "Error from cross_two_individuals : second_parent is null\n");
        return;
    }

    /* Prepare the child for inserting the datas of its parents*/
    init_individual(child);

    child->hospitals_size = (first_parent->hospitals_size + second_parent->hospitals_size) / 2;

    child->hospitals = malloc(child->hospitals_size * sizeof(Hospital));
    if (child->hospitals == NULL)
    {
        fprintf(stderr, "Error from cross_two_individuals: Memory allocation failed\n");
        return;
    }

    int i;
    for (i = 0; i < child->hospitals_size; i++)
    {
        init_hospital(&(child->hospitals[i]));
    }

    /* Filling the child hospitals array */
    for (i = 0; i < child->hospitals_size; i++)
    {
        Hospital *h_dest = &(child->hospitals[i]);
        const Individual *current_parent;
        int parent_hopitals_size;

        /* Alternate parents */
        if (i % 2 == 0)
        {
            current_parent = first_parent;
        }
        else
        {
            current_parent = second_parent;
        }
        parent_hopitals_size = current_parent->hospitals_size;

        /* To get as much element from the beginning of the parents list thant from the end */
        int idx_p = 0;
        if (parent_hopitals_size > 1)
        {
            idx_p = (i * (double)(parent_hopitals_size - 1) / (child->hospitals_size - 1));
        }

        h_dest->location = current_parent->hospitals[idx_p].location;
    }

    evaluate_individual(child, cities, cities_size);
}


/**
 * @brief Crée un nouvel individu en mutant un parent.
 */
void mutate_individual(Individual* child, const Individual* parent, City* cities, int cities_size) {
    if (!child || !parent) return;

    copy_individual(child, parent, cities, cities_size);

    if (child->hospitals_size > 0) {
        int target_idx = rand() % child->hospitals_size;
        
        int new_city_idx = rand() % cities_size;
        child->hospitals[target_idx].location = &cities[new_city_idx];
        
        evaluate_individual(child, cities, cities_size);
    }
}