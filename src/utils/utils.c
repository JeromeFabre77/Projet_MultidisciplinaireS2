#include <stdio.h>
#include <stdlib.h>

#include "utils.h"

/**
 * @brief Dynamically allocates or resizes a City array.
 * @param cities Current array pointer (NULL for the initial allocation).
 * @param city_count Desired number of City elements.
 * @return A valid new pointer, or NULL on failure (or if city_count <= 0).
 */
City *city_resize_array(City *cities, int city_count) {
	City *resized;

	/* If the requested size is zero (or negative),
	 * free the existing array and indicate that there is no more data. */
	if (city_count <= 0) {
		free(cities);
		return NULL;
	}

	/* realloc handles both cases:
	 * - cities == NULL: behavior equivalent to malloc
	 * - cities != NULL: resize the existing block */
	resized = (City *)realloc(cities, city_count * sizeof(City));
	if (resized == NULL) {
		fprintf(stderr, "Error: Memory reallocation failed for cities array\n");
		return NULL;
	}

	return resized;
}