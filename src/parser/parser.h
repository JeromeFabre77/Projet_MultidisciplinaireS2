#ifndef __PARSER__
#define __PARSER__

#include <stdio.h>
#include "../city/city.h"

#define Folder = "../assets/results/"

/**
 * @brief Opens a CSV file in read mode
 * @param file_path Path to the CSV file
 * @return FILE pointer if successful, NULL on failure
 * @note Must be closed with close_csv_file() or fclose()
 */
FILE* open_csv_file(const char* file_path);

/**
 * @brief Closes a CSV file safely
 * @param file FILE pointer to close
 * @return 0 on success, EOF on failure
 */
int close_csv_file(FILE* file);

/**
 * @brief Gets file statistics in a single pass (optimized)
 * @param file_path Path to the CSV file
 * @param line_count Pointer to store the line count
 * @param max_length Pointer to store the max line length
 * @return 0 on success, -1 on failure
 */
int getFileStats(const char* file_path, int* line_count, int* max_length);

/**
 * @brief Loads all cities from a CSV file
 * @param file_path Path to the CSV file
 * @return Pointer to array of City structures, NULL on failure
 * @note Must be freed by the caller using free()
 */
City* get_cities_from_csv(char* file_path);

/**
 * @brief Prints all cities to stdout in a formatted table
 * @param cities Pointer to array of City structures
 * @param count Number of cities to print (0 for all until null terminator)
 */
void printCities(const City *cities, int count);

#endif