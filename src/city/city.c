#include <stdio.h>
#include <stdlib.h>
#include "city.h"

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
