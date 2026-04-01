#include <stdio.h>
#include <stdlib.h>
#include "interface/interface.h"
#include "parser/parser.h"
#include "city/city.h"

int main(void) {
    City *cities = NULL;
    int city_count;

    city_count = get_cities_from_csv("../assets/data/communes-france-metrople-2025.csv", &cities);

    if (city_count < 0 || cities == NULL) {
        fprintf(stderr, "Error: Failed to load cities from CSV\n");
        return 1;
    }

    printf("=== First 10 Cities ===\n");
    print_cities(cities, city_count, 10);

    free(cities);

    return 0;
}