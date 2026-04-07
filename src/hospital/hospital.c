#include <stdio.h>
#include <stdlib.h>

#include "hospital.h"


#define CHANCES_MIN_TO_PLACE_HOSPITAL 10 /* Percentage minimal of chances to associate an hospital to a city when creating randomly, on 100 */
#define CHANCES_MAX_TO_PLACE_HOSPITAL 20 /* Percentage maximal of chances to associate an hospital to a city when creating randomly, on 100 */

/**
 * @brief Initialize an hospital with null or 0 values. It MUST be use after a memory allocation for create an hospital.
 * @param hospital The pointer to the hospital to initialize
 */
void init_hospital(Hospital* hospital)
{
    if (hospital == NULL)
    {
        fprintf(stderr, "Error from init_hospital: hospital is null\n");
        return;
    }

    hospital->location = NULL;
    hospital->is_chru = 0;
    hospital->bed_count = 0;
    hospital->cities = NULL;
    hospital->cities_size = 0;
    hospital->covered_population = 0;
}

/**
 * @brief Generate a random array of hospital in cities.
 * 
 * It cross all the cities array and for each city there is a chance associate an hospital to it. This chances is defined 
 * by the settings CHANCES_MIN_TO_PLACE_HOSPITAL and CHANCES_MAX_TO_PLACE_HOSPITAL.
 * 
 * 
 * @param hospitals The pointer to the begin of the hospitals list that will be generated
 * @param hospitals_size The pointer to the size of the hospitals list that will be generated
 * @param cities The array of all cities
 * @param cities_size The size of the array of all cities
 */
void create_random_list_of_hospital(Hospital **hospitals, int *hospitals_size, City *cities, int cities_size)
{
    if (hospitals == NULL)
    {
        fprintf(stderr, "Error from create_random_list_of_hospital : hospitals is null\n");
        return;
    }
    if (hospitals_size == NULL)
    {
        fprintf(stderr, "Error from create_random_list_of_hospital : hospitals_size is null\n");
        return;
    }
    if (cities == NULL)
    {
        fprintf(stderr, "Error from create_random_list_of_hospital : cities is null\n");
        return;
    }
    if (cities_size <= 0)
    {
        fprintf(stderr, "Error from create_random_list_of_hospital : cities_size is 0\n");
        return;
    }

    /* To store temporary indexes of city that will be associate to an hospital */
    int *selected_indices = malloc(cities_size * sizeof(int));
    if (selected_indices == NULL)
    {
        fprintf(stderr, "Error from create_random_list_of_hospital : Memory allocation failed\n");
        return;
    }

    int hospital_count = 0;

    /* Random draw to associate or not an hospital to a city */
    int i;
    for (i = 0; i < cities_size; i++)
    {
        int draw = rand() % 100;
        if (draw > CHANCES_MIN_TO_PLACE_HOSPITAL && draw < CHANCES_MAX_TO_PLACE_HOSPITAL)
        {
            selected_indices[hospital_count] = i;
            hospital_count++;
        }
    }

    if (hospital_count == 0)
    {
        free(selected_indices);
        fprintf(stderr, "Error from create_random_list_of_hospital : hospital_count is 0, your bounds for creating a random list of hospital are to low\n");
        return;
    }

    /* Allocate memory for the hospitals array */
    *hospitals = malloc(hospital_count * sizeof(Hospital));
    if (*hospitals == NULL)
    {
        free(selected_indices);
        fprintf(stderr, "Error from create_random_list_of_hospital : Memory allocation failed\n");
        return;
    }

    /* Fill the hospitals array */
    for (i = 0; i < hospital_count; i++)
    {
        init_hospital(&((*hospitals)[i]));

        (&((*hospitals)[i]))->location = &cities[selected_indices[i]];
    }

    free(selected_indices);

    *hospitals_size = hospital_count;
}


