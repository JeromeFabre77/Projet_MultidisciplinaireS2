#include "interface/interface.h"
#include "parser/parser.h"

int main(int argc, char *argv[])
{
    /* Load cities from CSV file (relative path from src directory) */
    City *cities = get_cities_from_csv("../assets/data/communes-france-metrople-2025.csv");
    
    if (cities == NULL)
    {
        fprintf(stderr, "Error: Failed to load cities from CSV\n");
        return 1;
    }
    printf("=== First 10 Cities ===\n");
    printCities(cities, 10);

    free(cities);

    /*
    create_window();

    while (1)
    {
        actualise_window();
    }

    free_window();*/
    return 0;
}