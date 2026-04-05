#include <stdio.h>
#include <stdlib.h>

#include "individual.h"

#define BED_FOR_POPULATION 5.4 /* Number of bed needed for 1000 people covered by an hospital*/



void evaluate_individual(Individual* individual, City* cities, int cities_size) {
    if (individual == NULL)  {
      fprintf(stderr, "Error from evaluate_individual: individual is null\n");
      return;
    }
    if (cities == NULL)  {
      fprintf(stderr, "Error from evaluate_individual : cities is null\n");
      return;
    }
    if (cities_size <=0)  {
      fprintf(stderr, "Error from evaluate_individual : cities_size is 0\n");
      return;
    }


    /* Temporary variables to get a fast access to informations */
    int* hospital_at_city = malloc(cities_size * sizeof(int)); /* Is an hospital present in a city, -1 if no, the index of the hospital if yes*/
    int* city_to_nearest_hospital = malloc(cities_size * sizeof(int)); /* What is the nearest hospital from a city, -1 if no hospital, the index of the hospital nearest from the city */
    int* nb_city_attached_to_hospital = calloc(individual->hospitals_size, sizeof(int)); /* Number of cities attached to each hospital */
    if (!hospital_at_city || !city_to_nearest_hospital || !nb_city_attached_to_hospital) {
        fprintf(stderr, "Error from evaluate_individual : Memory allocation failed during evaluation\n");
        return;
    }

    int i;
    for (i = 0; i < cities_size; i++) {
        hospital_at_city[i] = -1;
        city_to_nearest_hospital[i] = -1;
    }


    /* Fill the hospital_at_city variable and count the number of CHRU*/
    for (i = 0; i < individual->hospitals_size; i++) {
        int idx = (int)(individual->hospitals[i].location - cities);
        hospital_at_city[idx] = i;
    
        if (individual->hospitals[i].is_chru) {
            individual->chru_count++;
        }
    }


    /* Fill the city_to_nearest_hospital variable */
    for (i = 0; i < cities_size; i++) {
        int nearest_hospital_idx = -1;
        
        /* If there is an hospital in the city, this hospital is the nearest for sure */
        if (hospital_at_city[i] != -1) {
            nearest_hospital_idx = hospital_at_city[i];
        } else {
            float min_distance = 1e9;

            /* For each neighbor of the city, check if there is an hospital in it, if no the city is isolated, 
            if yes find the one with the minimal distance*/
            int j;
            for (j = 0; j < cities[i].neighbors_size; j++) {
                int neighbor_idx = (int)(cities[i].neighbors[j].city - cities);

                if (hospital_at_city[neighbor_idx] != -1) {
                    if (cities[i].neighbors[j].distance < min_distance) {
                        min_distance = cities[i].neighbors[j].distance;
                        nearest_hospital_idx = hospital_at_city[neighbor_idx];
                    }
                }
            }
        }
        
        city_to_nearest_hospital[i] = nearest_hospital_idx;

        if (nearest_hospital_idx != -1) {
            nb_city_attached_to_hospital[nearest_hospital_idx]++;
        }
    }

    /* Allocate the memory of cities array for each hospital */
    for (i = 0; i < individual->hospitals_size; i++) {
        if (nb_city_attached_to_hospital[i] > 0) {
            individual->hospitals[i].cities = malloc(nb_city_attached_to_hospital[i] * sizeof(City*));
            individual->hospitals[i].cities_size = 0; /* Set to 0 for the moment to use it as a index will filling */
        }
    }

    
    /* Fill the cities array of each hospital by crossing each cities and check which hospital is the nearest */
    int total_covered_pop = 0;
    for (i = 0; i < cities_size; i++) {
        int hospital_idx = city_to_nearest_hospital[i];

        if (hospital_idx != -1) {
            individual->hospitals[hospital_idx].cities[individual->hospitals[hospital_idx].cities_size] = &cities[i];
      
            individual->hospitals[hospital_idx].covered_population += cities[i].population;
  
            individual->hospitals[hospital_idx].cities_size++;

            total_covered_pop += cities[i].population;
        } else {
            individual->isolated_population += cities[i].population;
        }
    }

    /* Evaluate last statistics not already done */
    individual->total_population = total_covered_pop;
    individual->score = individual->total_population - individual->isolated_population - (PENALTY_HOSPITAL * individual->hospitals_size) + (BONUS_CHRU * individual->chru_count);

    for(i=0; i<individual->hospitals_size; i++) {
      individual->hospitals[i].bed_count = (individual->hospitals[i].covered_population/1000) * BED_FOR_POPULATION;
    }


    /* Free temporary variables */
    free(hospital_at_city);
    free(city_to_nearest_hospital);
    free(nb_city_attached_to_hospital);
}





void create_random_individual(Individual* individual, City *cities, int cities_size)
{
  if (!individual)
  {
    fprintf(stderr, "Error: individual is null, memory allocation failed\n");
    return;
  }

  int err_code = create_random_list_of_hospital(&(individual->hospitals), &individual->hospitals_size, cities, cities_size);
  if (err_code != 0)
  {
    fprintf(stderr, "Error: Failed to create a random list of hospital\n");
    return;
  }

  individual->chru_count = 0;
  int i;
  for(i=0; i<individual->hospitals_size; i++) {
    if(individual->hospitals[i].is_chru) {
      individual->chru_count++;
    }
  }


  individual->score = 0;
  individual->isolated_population = 0;

  evaluate_individual(individual, cities, cities_size);
  
}


/**
 * @brief Compare deux individus par leur score pour le tri.
 * Ordre décroissant : le score le plus élevé arrive en premier (index 0).
 */
int compare_individuals(const void* a, const void* b) {
    /* Cast des pointeurs génériques en pointeurs d'Individual */
    const Individual* indA = (const Individual*)a;
    const Individual* indB = (const Individual*)b;

    if (indB->score > indA->score) {
        return 1;
    } else if (indB->score < indA->score) {
        return -1;
    } else {
        return 0;
    }
}










/* Temporary */
void print_individual(const Individual *individual)
{
  if (individual == NULL)
  {
    printf("Individual is NULL.\n");
    return;
  }

  printf("============= An individual =============\n");
  printf("Score (Fitness)      : %d\n", individual->score);
  printf("Hospitals Count      : %d\n", individual->hospitals_size);
  printf("CHRU Count           : %d\n", individual->chru_count);
  printf("Total Population  : %d\n", individual->total_population);
  printf("Isolated Population  : %d\n", individual->isolated_population);
  printf("Hospital List (5 firsts ...):\n");
  print_hospitals(individual->hospitals, individual->hospitals_size, 5);

  printf("\n\n");
}