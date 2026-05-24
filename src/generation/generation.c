#include "generation.h"

#include <stdlib.h>
#include <stdio.h>

#include "../constants.h"
#include "../utils/utils.h"

/**
 * @brief Allocates and initializes a new Generation containing a fixed number of empty individuals.
 * @param number The sequence number or ID of the generation.
 * @param individuals_size The total number of individuals to create within this generation.
 * @return A pointer to the newly created generation structure.
 */
Generation *create_new_generation(int number, int individuals_size)
{
    Generation *gen = (Generation *)malloc(sizeof(Generation));
    if (gen == NULL)
    { /* Here have to use explicitly a if an not the utilitary to avoid warning */
        fprintf(stderr, "Error: create_new_generation() -> allocating Generation struct\n");
        exit(EXIT_FAILURE);
    }

    gen->number = number;
    gen->individuals_size = individuals_size;

    gen->individuals = NULL;
    gen->individuals = malloc(individuals_size * sizeof(Individual *));
    check_allocation(gen->individuals, "create_new_generation() -> allocating individuals pointer array");

    int i = 0;
    for (i = 0; i < individuals_size; i++)
    {
        gen->individuals[i] = NULL;
        gen->individuals[i] = create_new_individual();
        check_allocation(gen->individuals[i], "create_new_generation() -> allocating individual");
    }

    return gen;
}

/**
 * @brief Safely frees a generation structure, all its individuals, and associated resources.
 * @param gen A double pointer to the generation to be freed, which will be set to NULL.
 */
void free_generation(Generation **gen)
{
    if (gen == NULL || *gen == NULL)
    {
        return;
    }

    if ((*gen)->individuals != NULL)
    {
        int i;
        for (i = 0; i < (*gen)->individuals_size; i++)
        {
            free_individual(&((*gen)->individuals[i]));
        }
        free((*gen)->individuals);
        (*gen)->individuals = NULL;
    }

    free(*gen);
    *gen = NULL;
}

/**
 * @brief Sorts the individuals of a generation by their fitness score in descending order using qsort.
 * @param gen A pointer to the generation structure whose individuals will be sorted in-place.
 */
void sort_generation_individuals_by_fitness(Generation *gen)
{
    if (gen == NULL || gen->individuals == NULL || gen->individuals_size <= 0)
    {
        fprintf(stderr, "Warning from sort_generation_individuals_by_fitness: Generation is empty or null\n");
        return;
    }

    qsort(gen->individuals, gen->individuals_size, sizeof(Individual *), compare_individuals);

    printf("Best score for generation %d: %d\n", gen->number, gen->individuals[0]->score);
    print_individual(gen->individuals[0]);
}

/**
 * @brief Selects an individual from the current generation using a tournament selection strategy.
 * @param current_gen A pointer to the constant generation from which to select.
 * @param tournament_size The number of random individuals chosen to compete against each other.
 * @return The index of the winning individual with the highest score.
 */
int run_tournament(const Generation *current_gen, const int tournament_size)
{
    int best_idx = rand() % INDIVIDUALS_SIZE;

    int i;
    for (i = 1; i < tournament_size; i++)
    {
        int challenger_idx = rand() % INDIVIDUALS_SIZE;
        if (current_gen->individuals[challenger_idx]->score > current_gen->individuals[best_idx]->score)
        {
            best_idx = challenger_idx;
        }
    }
    return best_idx;
}