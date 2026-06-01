#ifndef __FILE_WRITER__
#define __FILE_WRITER__

#include "../individual/individual.h"

#define RESULT_FOLDER "../assets/results/"

void save_individual_and_cities_in_JSON(char* filepath, Individual* individual, City* cities, int cities_size);


#endif