#include <stdio.h>
#include <stdlib.h>
#include "city.h"

/**
 * @brief Alloue ou reajuste dynamiquement un tableau de City.
 * @param cities Pointeur courant du tableau (NULL pour une premiere allocation).
 * @param city_count Nombre d'elements City souhaites.
 * @return Nouveau pointeur valide, ou NULL en cas d'echec (ou si city_count <= 0).
 */
City *city_resize_array(City *cities, int city_count) {
	City *resized;

	/* Si la taille demandee est nulle (ou negative),
	 * on libere le tableau existant et on indique qu'il n'y a plus de donnees. */
	if (city_count <= 0) {
		free(cities);
		return NULL;
	}

	/* realloc couvre les deux cas:
	 * - cities == NULL : comportement equivalent a malloc
	 * - cities != NULL : redimensionnement du bloc existant */
	resized = (City *)realloc(cities, city_count * sizeof(City));
	if (resized == NULL) {
		fprintf(stderr, "Error: Memory reallocation failed for cities array\n");
		return NULL;
	}

	return resized;
}

/* Temporary function kept for debugging/preview until UI rendering fully replaces terminal output. */
void print_cities(const City *cities, int city_count, int count_to_print) {
	int i;
	int limit;

	if (cities == NULL) {
		fprintf(stderr, "Error: cities pointer is NULL\n");
		return;
	}

	if (city_count < 0) {
		fprintf(stderr, "Error: city_count is invalid\n");
		return;
	}

	limit = city_count;
	if (count_to_print > 0 && count_to_print < city_count) {
		limit = count_to_print;
	}

	printf("┌─────────┬──────────────────────────┬──────────────────┬────────────┬────────────┐\n");
	printf("│ INSEE   │ City Name                │ Region           │ Population │ Lat/Lng    │\n");
	printf("├─────────┼──────────────────────────┼──────────────────┼────────────┼────────────┤\n");

	for (i = 0; i < limit; i++) {
		printf("│ %-7s │ %-24s │ %-16s │ %10d │ %.1f/%.1f │\n",
			   cities[i].insee_code,
			   cities[i].name,
			   cities[i].region_name,
			   cities[i].population,
			   cities[i].latitude,
			   cities[i].longitude);
	}

	printf("└─────────┴──────────────────────────┴──────────────────┴────────────┴────────────┘\n");
	printf("Total: %d cities displayed\n", limit);
}
