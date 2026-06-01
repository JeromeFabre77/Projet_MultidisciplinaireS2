#include "individual.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "../constants.h"
#include "../utils/utils.h"

/**
 * @brief Allocate and initialize a new individual.
 * Memory is allocated for the Individual structure, and its internal fields
 * are initialized to their default values (zeroed out or set to NULL).
 * @return A pointer to the newly created Individual.
 */
Individual *create_new_individual(void)
{
    Individual *individual = (Individual *)malloc(sizeof(Individual));
    if (individual == NULL)
    { /* Here have to use explicitly a if an not the utilitary to avoid warning */
        fprintf(stderr, "Error: create_new_individual() -> allocating individual failed\n");
        exit(EXIT_FAILURE);
    }

    individual->score = 0;
    individual->hospitals = NULL;
    individual->hospitals_size = 0;
    individual->chru_count = 0;
    individual->isolated_population = 0;
    individual->covered_population = 0;

    return individual;
}

/**
 * @brief Safely free the memory allocated for an individual.
 * Iterates through and frees all associated hospitals inside the individual,
 * then frees the individual structure itself, and finally sets the pointer to NULL.
 * @param individual A pointer to the pointer of the individual to be freed.
 */
void free_individual(Individual **individual)
{
    if (individual == NULL || *individual == NULL)
        return;

    if ((*individual)->hospitals != NULL)
    {
        int i;
        for (i = 0; i < (*individual)->hospitals_size; i++)
        {
            free_hospital(&((*individual)->hospitals[i]));
        }
        free((*individual)->hospitals);
    }

    free(*individual);
    *individual = NULL;
}

/**
 * @brief Compare two individuals based on their score. Useful for qsort (descending order).
 * @param a Pointer to the first individual pointer.
 * @param b Pointer to the second individual pointer.
 * @return An integer less than, equal to, or greater than zero if the first
 * individual's score is found, respectively, to be greater than,
 * equal to, or less than the second.
 */
int compare_individuals(const void *a, const void *b)
{
    const Individual *indA = *(const Individual **)a;
    const Individual *indB = *(const Individual **)b;

    if (indB->score > indA->score)
        return 1;
    if (indB->score < indA->score)
        return -1;
    return 0;
}

/**
 * @brief Browse the hospital list of an individual and count how many are CHRUs.
 * @param individual The constant individual to analyze.
 * @return The total number of CHRUs found.
 */
int count_number_of_chru_in_individual(const Individual *individual)
{
    check_parameter(individual, "count_number_of_chru_in_individual() -> parameter 'individual' is null");

    int chru_count = 0;

    int i;
    for (i = 0; i < individual->hospitals_size; i++)
    {
        if (individual->hospitals[i]->is_chru == 1)
        {
            chru_count++;
        }
    }

    return chru_count;
}

/**
 * @brief Calculate the total covered population across all hospitals of an individual.
 * @param individual The constant individual to analyze.
 * @return The sum of the covered population for all hospitals.
 */
int count_covered_population_in_individual(const Individual *individual)
{
    check_parameter(individual, "count_covered_population_in_individual() -> parameter 'individual' is null");

    int total_covered_population = 0;

    int i;
    for (i = 0; i < individual->hospitals_size; i++)
    {
        total_covered_population += individual->hospitals[i]->covered_population;
    }

    return total_covered_population;
}

/**
 * @brief Dispatch all cities to their nearest hospital based on neighbor distances.
 * It uses an optimized lookup table (dictionary) to map cities to hospitals,
 * checking first if the city itself has a hospital, and then checking its neighbors.
 * @param individual The individual containing the hospitals to populate.
 * @param cities The array of all available cities.
 * @param cities_size The total number of cities in the array.
 */
void dispatch_cities_to_hospitals(Individual *individual, const City *cities, const int cities_size)
{
    check_parameter(individual, "dispatch_cities_to_hospitals() -> parameter 'individual' is null");
    check_parameter(cities, "dispatch_cities_to_hospitals() -> parameter 'cities' is null");

    /* 2. Create the "city -> hospital" lookup table for constant time access */
    int *hospital_at_city = malloc(cities_size * sizeof(int));
    check_allocation(hospital_at_city, "dispatch_cities_to_hospitals() -> allocating hospital_at_city");

    int j;
    for (j = 0; j < cities_size; j++)
    {
        hospital_at_city[j] = -1;
    }
    int h;
    for (h = 0; h < individual->hospitals_size; h++)
    {
        /* Using pointer arithmetic to find the city index */
        int city_idx = (int)(individual->hospitals[h]->location - cities);
        hospital_at_city[city_idx] = h;
    }

    /* 3. Distribution: Single and optimized iteration over all cities */
    int i;
    for (i = 0; i < cities_size; i++)
    {
        int nearest_hospital_idx = -1;
        double min_distance = 1e9;

        /* A. Does the city itself host a hospital? (Unbeatable distance of 0) */
        if (hospital_at_city[i] != -1)
        {
            nearest_hospital_idx = hospital_at_city[i];
            min_distance = 0.0;
        }
        else
        {
            /* B. Otherwise, inspect only the circle of neighbors within 10km */
            int j;
            for (j = 0; j < cities[i].neighbors_size; j++)
            {
                int neighbor_idx = (int)(cities[i].neighbors[j].city - cities);
                int potential_hospital_idx = hospital_at_city[neighbor_idx];

                /* If the neighbor hosts a hospital and is closer */
                if (potential_hospital_idx != -1)
                {
                    if (cities[i].neighbors[j].distance < min_distance)
                    {
                        min_distance = cities[i].neighbors[j].distance;
                        nearest_hospital_idx = potential_hospital_idx;
                    }
                }
            }
        }

        /* C. Verdict: Assign to the winning hospital, or mark as isolated */
        if (nearest_hospital_idx != -1)
        {
            add_covered_city_to_hospital(individual->hospitals[nearest_hospital_idx], &cities[i]);
        }
    }

    /* 4. Free the temporary lookup table */
    free(hospital_at_city);
}

/**
 * @brief Evaluate the fitness score and statistics of an individual.
 * This function dispatches cities to the individual's hospitals, evaluates each
 * hospital individually, computes the overall covered/isolated population, and
 * calculates the final score incorporating hospital penalties and CHRU bonuses.
 * @param individual The individual to evaluate (fields will be updated).
 * @param cities The constant array of all available cities.
 * @param cities_size The total number of cities in the array.
 * @param total_population The total population of all cities combined.
 */
void evaluate_individual(Individual *individual, const City *cities, const int cities_size, const int total_population)
{
    check_parameter(individual, "evaluate_individual() -> parameter 'individual' is null");
    check_parameter(cities, "evaluate_individual() -> parameter 'cities' is null");

    dispatch_cities_to_hospitals(individual, cities, cities_size);

    int i;
    for (i = 0; i < individual->hospitals_size; i++)
    {
        evaluate_hospital(individual->hospitals[i]);
    }

    individual->chru_count = count_number_of_chru_in_individual(individual);
    individual->covered_population = count_covered_population_in_individual(individual);
    individual->isolated_population = total_population - individual->covered_population;

    individual->score = total_population - (PENALTY_ISOLATED_POP * individual->isolated_population) - (PENALTY_HOSPITAL * individual->hospitals_size) + (BONUS_CHRU * individual->chru_count);
}

/**
 * @brief Add a new hospital at a specific city location to an individual.
 * The function first checks for duplicates to prevent adding a hospital
 * at the same location twice. If unique, it resizes the hospital array
 * and initializes the new hospital.
 * @param individual The individual to whom the hospital will be added.
 * @param location The constant city location where the hospital will be placed.
 */
void add_hospital_to_individual(Individual *individual, const City *location)
{
    check_parameter(individual, "add_hospital_to_individual() -> parameter 'individual' is null");
    check_parameter(location, "add_hospital_to_individual() -> parameter 'location' is null");

    /* Avoid duplicates */
    if (individual->hospitals != NULL)
    {
        int i;
        for (i = 0; i < individual->hospitals_size; i++)
        {
            if (individual->hospitals[i]->location == location)
            {
                return;
            }
        }
    }

    individual->hospitals = realloc(individual->hospitals, (individual->hospitals_size + 1) * sizeof(Hospital *));
    check_allocation(individual->hospitals, "add_hospital_to_individual() -> reallocating individual->hospitals");

    individual->hospitals[individual->hospitals_size] = create_new_hospital(location);

    individual->hospitals_size++;
}

/**
 * @brief Takes an empty individual, fills it with a random list of hospitals, and evaluates it.
 * @param individual A pointer to the individual to populate and evaluate.
 * @param cities The constant array of all available cities.
 * @param cities_size The total number of cities in the array.
 * @param total_population The total population of all cities combined.
 */
void generate_random_individual(Individual *individual, const City *cities, const int cities_size, const int total_population)
{
    check_parameter(individual, "create_random_individual() -> parameter 'individual' is null");
    check_parameter(cities, "create_random_individual() -> parameter 'cities' is null");

    int target_percentage = CHANCES_MIN_TO_PLACE_HOSPITAL + (rand() % (CHANCES_MAX_TO_PLACE_HOSPITAL - CHANCES_MIN_TO_PLACE_HOSPITAL + 1));
    int target_hospital_count = (cities_size * target_percentage) / 100;
    if (target_hospital_count <= 0)
    {
        target_hospital_count = 1;
    }

    int *selected_city_indices = malloc(target_hospital_count * sizeof(int));
    check_allocation(selected_city_indices, "create_random_individual() -> allocating selected_city_indices");

    int count = 0;
    while (count < target_hospital_count)
    {
        int random_city_idx = rand() % cities_size;
        int already_exists = 0;
        int i;
        for (i = 0; i < count; i++)
        {
            if (selected_city_indices[i] == random_city_idx)
            {
                already_exists = 1;
                break;
            }
        }

        if (!already_exists)
        {
            selected_city_indices[count] = random_city_idx;
            count++;
        }
    }

    int i;
    for (i = 0; i < target_hospital_count; i++)
    {
        int city_idx = selected_city_indices[i];
        add_hospital_to_individual(individual, &cities[city_idx]);
    }

    free(selected_city_indices);

    evaluate_individual(individual, cities, cities_size, total_population);
}

/**
 * @brief Takes an empty individual and copies the hospital list of a source individual into it, then evaluates it.
 * @param dest A pointer to the destination individual to be populated and evaluated.
 * @param src A constant pointer to the source individual to copy from.
 * @param cities The constant array of all available cities.
 * @param cities_size The total number of cities in the array.
 * @param total_population The total population of all cities combined.
 */
void copy_individual(Individual *dest, const Individual *src, const City *cities, const int cities_size, const int total_population)
{
    check_parameter(dest, "copy_individual() -> parameter 'dest' is null");
    check_parameter(src, "copy_individual() -> parameter 'src' is null");
    check_parameter(cities, "copy_individual() -> parameter 'cities' is null");

    dest->hospitals_size = src->hospitals_size;
    dest->hospitals = malloc(dest->hospitals_size * sizeof(Hospital *));
    check_allocation(dest->hospitals, "copy_individual() -> allocating dest->hospitals");

    int i;
    for (i = 0; i < dest->hospitals_size; i++)
    {
        dest->hospitals[i] = create_new_hospital(src->hospitals[i]->location);
    }

    evaluate_individual(dest, cities, cities_size, total_population);
}

/**
 * @brief Cross two parent individuals by alternating their hospitals to create a child.
 * * This crossover operator interleaves hospitals from both parents (one by one).
 * If one parent has fewer hospitals than the other, the remaining hospitals
 * from the larger parent are appended at the end.
 * @param child            A pointer to the empty destination individual.
 * @param first_parent     A constant pointer to the first parent individual.
 * @param second_parent    A constant pointer to the second parent individual.
 * @param cities           The constant array of all available cities.
 * @param cities_size      The total number of cities in the array.
 * @param total_population The total population of all cities combined.
 */
void cross_two_individuals(Individual *child, const Individual *first_parent, const Individual *second_parent, const City *cities, const int cities_size, const int total_population)
{
    check_parameter(child, "cross_two_individuals() -> parameter 'child' is null");
    check_parameter(first_parent, "cross_two_individuals() -> parameter 'first_parent' is null");
    check_parameter(second_parent, "cross_two_individuals() -> parameter 'second_parent' is null");
    check_parameter(cities, "cross_two_individuals() -> parameter 'cities' is null");

    int idx1 = 0;
    int idx2 = 0;
    int turn = 0;
    while (idx1 < first_parent->hospitals_size || idx2 < second_parent->hospitals_size)
    {
        if ((turn == 0 && idx1 < first_parent->hospitals_size) || (idx2 >= second_parent->hospitals_size))
        {
            add_hospital_to_individual(child, first_parent->hospitals[idx1]->location);
            idx1++;
            turn = 1; /* turn to the second parent */
        }
        else if ((turn == 1 && idx2 < second_parent->hospitals_size) || (idx1 >= first_parent->hospitals_size))
        {
            add_hospital_to_individual(child, second_parent->hospitals[idx2]->location);
            idx2++;
            turn = 0; /* turn to the first parent */
        }
    }

    evaluate_individual(child, cities, cities_size, total_population);
}

/**
 * @brief Mutate an individual to produce a child based on random mutation chances.
 * For each hospital in the parent, a random draw determines if it is:
 * - Removed (not copied to the child).
 * - Kept alongside a newly added random hospital.
 * - Kept normally without any changes.
 * @param child            A pointer to the empty destination individual.
 * @param parent           A constant pointer to the parent individual to mutate.
 * @param cities           The constant array of all available cities.
 * @param cities_size      The total number of cities in the array.
 * @param total_population The total population of all cities combined.
 */
void mutate_individual(Individual *child, const Individual *parent, const City *cities, const int cities_size, const int total_population)
{
    check_parameter(child, "mutate_individual() -> parameter 'child' is null");
    check_parameter(parent, "mutate_individual() -> parameter 'parent' is null");
    check_parameter(cities, "mutate_individual() -> parameter 'cities' is null");

    int i;
    for (i = 0; i < parent->hospitals_size; i++)
    {
        int draw = rand() % 100;

        if (draw < MUTATION_CHANCES_TO_REMOVE)
        {
            /* Nothing to do, we just do not copy the hospital in the child */
        }
        else if (draw < (MUTATION_CHANCES_TO_REMOVE + MUTATION_CHANCES_TO_ADD_NEW))
        {
            int random_city_idx = rand() % cities_size;
            add_hospital_to_individual(child, &cities[random_city_idx]);
            add_hospital_to_individual(child, parent->hospitals[i]->location);
        }
        else
        {
            add_hospital_to_individual(child, parent->hospitals[i]->location);
        }
    }

    evaluate_individual(child, cities, cities_size, total_population);
}

/**
 * @brief Print the statistics and fitness score of an individual to the standard output.
 * @param individual The constant individual to print.
 */
void print_individual(const Individual *individual)
{
    check_parameter(individual, "print_individual() -> parameter 'individual' is null");

    printf("Individual :\n");
    printf("fitness : %d\n", individual->score);
    printf("hospitals_size : %d\n", individual->hospitals_size);
    printf("isolated_population : %d\n", individual->isolated_population);
    printf("covered_population : %d\n", individual->covered_population);
    printf("chru_count : %d\n", individual->chru_count);
}

/**
 * @brief Check if an individual already has a hospital at a given city location.
 *
 * @param individual The individual to search into.
 * @param location The city location to check.
 * @return 1 if the individual has a hospital at this location, 0 otherwise.
 */
int contains_hospital_at_location(const Individual *individual, const City *location)
{
    check_parameter(individual, "contains_hospital_at_location() -> parameter 'individual' is null");
    check_parameter(location, "contains_hospital_at_location() -> parameter 'location' is null");

    int i;
    for (i = 0; i < individual->hospitals_size; i++)
    {
        if (individual->hospitals[i]->location == location)
        {
            return 1;
        }
    }

    return 0;
}

/**
 * @brief Cross six individuals by keeping only hospitals shared by at least two parents.
 *
 * A hospital is considered the same if it has the same city location pointer.
 *
 * @param child The child individual to fill.
 * @param first_parent The first parent individual.
 * @param second_parent The second parent individual.
 * @param third_parent The third parent individual.
 * @param fourth_parent The fourth parent individual.
 * @param fifth_parent The fifth parent individual.
 * @param sixth_parent The sixth parent individual.
 * @param cities The array of cities.
 * @param cities_size The size of the cities array.
 * @param total_population The total population.
 */
void cross_six_individuals(
    Individual *child,
    const Individual *first_parent,
    const Individual *second_parent,
    const Individual *third_parent,
    const Individual *fourth_parent,
    const Individual *fifth_parent,
    const Individual *sixth_parent,
    const City *cities,
    const int cities_size,
    const int total_population)
{
    check_parameter(child, "cross_six_individuals() -> parameter 'child' is null");
    check_parameter(first_parent, "cross_six_individuals() -> parameter 'first_parent' is null");
    check_parameter(second_parent, "cross_six_individuals() -> parameter 'second_parent' is null");
    check_parameter(third_parent, "cross_six_individuals() -> parameter 'third_parent' is null");
    check_parameter(fourth_parent, "cross_six_individuals() -> parameter 'fourth_parent' is null");
    check_parameter(fifth_parent, "cross_six_individuals() -> parameter 'fifth_parent' is null");
    check_parameter(sixth_parent, "cross_six_individuals() -> parameter 'sixth_parent' is null");
    check_parameter(cities, "cross_six_individuals() -> parameter 'cities' is null");

    const Individual *parents[6] = {
        first_parent,
        second_parent,
        third_parent,
        fourth_parent,
        fifth_parent,
        sixth_parent};

    int parent_index;
    int hospital_index;

    for (parent_index = 0; parent_index < 6; parent_index++)
    {
        const Individual *current_parent = parents[parent_index];

        for (hospital_index = 0; hospital_index < current_parent->hospitals_size; hospital_index++)
        {
            const City *location = current_parent->hospitals[hospital_index]->location;

            /*
             * If the hospital has already been added to the child,
             * it has already been handled.
             */
            if (contains_hospital_at_location(child, location))
            {
                continue;
            }

            int count = 0;
            int compare_index;

            for (compare_index = 0; compare_index < 6; compare_index++)
            {
                if (contains_hospital_at_location(parents[compare_index], location))
                {
                    count++;
                }
            }

            if (count >= 2)
            {
                add_hospital_to_individual(child, location);
            }
        }
    }

    evaluate_individual(child, cities, cities_size, total_population);
}