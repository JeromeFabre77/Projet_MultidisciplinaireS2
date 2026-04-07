#include <stdio.h>
#include <stdlib.h>

#include "hospital.h"

#include "../city/city.h"


#define CHANCES_MIN_TO_PLACE_HOSPITAL 10 /* Percentage minimal of chances to associate an hospital to a city when creating randomly, on 100 */
#define CHANCES_MAX_TO_PLACE_HOSPITAL 20 /* Percentage maximal of chances to associate an hospital to a city when creating randomly, on 100 */


void init_hospital(Hospital* hospital) {
    hospital->location = NULL;
    hospital->is_chru = 0;
    hospital->bed_count = 0;
    hospital->cities = NULL;
    hospital->cities_size = 0;
    hospital->covered_population = 0;
}


int create_random_list_of_hospital(Hospital** hospitals, int* hospitals_size, City* cities, int cities_size) {
  if (cities == NULL)
    {
        fprintf(stderr, "Error: cities is null\n");
        return -1;
    }
    if (cities_size <= 0)
    {
        fprintf(stderr, "Error: cities_size is 0 or minus\n");
        return -1;
    }

    /* To store temporary indexes of city that will be associate to an hospital */
    int *selected_indices = malloc(cities_size * sizeof(int));
    if (selected_indices == NULL) {
      fprintf(stderr, "Error: selected_indices is null\n");
      return -1;
    } 

    int hospital_count = 0;

    /* Random draw to associate or not an hospital to a city */
    int i;
    for (i = 0; i < cities_size; i++) {
        int draw= rand() % 100;
        if (draw > CHANCES_MIN_TO_PLACE_HOSPITAL && draw < CHANCES_MAX_TO_PLACE_HOSPITAL) {
            selected_indices[hospital_count] = i;
            hospital_count++;
        }
    }

    if (hospital_count == 0) {
        free(selected_indices);
        fprintf(stderr, "Error: hospital_count is 0, your bounds for creating a random list of hospital are to low\n");
        return -1;
    }

    /* Allocate memory for the hospitals array */
    *hospitals = malloc(hospital_count * sizeof(Hospital));
    if (*hospitals == NULL) {
        free(selected_indices);
        fprintf(stderr, "Error: hospitals is null, memory allocation failed\n");
        return -1;
    }

    /* Fill the hospitals array */
    for (i = 0; i < hospital_count; i++) {
        int city_idx = selected_indices[i];
        Hospital *h = &((*hospitals)[i]);

        h->location = &cities[city_idx];
        h->is_chru = 0;
        h->covered_population = 0;
        h->bed_count = 0;
    }

    free(selected_indices);

    *hospitals_size = hospital_count; 

    return 0;
}


void free_hospital_resources(Hospital *h) {
    if (h == NULL) return;

    if (h->cities != NULL) {
        free(h->cities);
        h->cities = NULL;
    }
}