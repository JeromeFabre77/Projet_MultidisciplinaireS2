#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../city/city.h"

/**
 * @brief Opens a CSV file in read mode with error checking
 * @param path Path to the CSV file to open
 * @return FILE pointer if successful, NULL on failure
 *
 * @note The returned FILE pointer must be closed by the caller using fclose()
 * @note Prints error message to stderr if file cannot be opened
 */
FILE *open_csv_file(const char *path)
{
    if (path == NULL)
    {
        fprintf(stderr, "Error: path is NULL\n");
        return NULL;
    }

    FILE *file = fopen(path, "r");
    if (file == NULL)
    {
        fprintf(stderr, "Error: Cannot open file '%s'\n", path);
        return NULL;
    }

    return file;
}

/**
 * @brief Safely closes a CSV file with error checking
 * @param file FILE pointer to close
 * @return 0 on success, EOF on failure
 */
int close_csv_file(FILE *file)
{
    if (file == NULL)
    {
        fprintf(stderr, "Warning: Attempting to close NULL file pointer\n");
        return 0;
    }

    if (fclose(file) == EOF)
    {
        fprintf(stderr, "Error: Failed to close file\n");
        return EOF;
    }

    return 0;
}

/**
 * @brief Gets file statistics in a single pass
 * @param path Path to the CSV file
 * @param line_count Pointer to store the line count
 * @param max_length Pointer to store the max line length
 * @return 0 on success, -1 on failure
 */
int getFileStats(const char *path, int *line_count, int *max_length)
{
    FILE *file = open_csv_file(path);
    if (file == NULL)
        return -1;

    *line_count = 0;
    *max_length = 0;

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
        (*line_count)++;
        int len = strlen(buffer);
        if (len > *max_length)
            *max_length = len;
    }

    close_csv_file(file);
    return 0;
}

/**
 * @brief Parses a single CSV line and fills a City structure
 * @param line The CSV line to parse (format: insee,name,region_code,region_name,dept_code,dept_name,postal_code,population,latitude,longitude)
 * @param city Pointer to City structure to fill
 * @return 0 on success, -1 on failure
 */
int parse_csv_line(const char *line, City *city)
{
    if (line == NULL || city == NULL)
    {
        fprintf(stderr, "Error: NULL pointer in parse_csv_line\n");
        return -1;
    }

    /* Create a copy to avoid modifying the original line */
    char *line_copy = (char *)malloc((strlen(line) + 1) * sizeof(char));
    if (line_copy == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return -1;
    }

    strcpy(line_copy, line);

    /* Remove trailing newline if present (\n) because fgets adds it */
    size_t len = strlen(line_copy);
    if (len > 0 && line_copy[len - 1] == '\n')
    {
        line_copy[len - 1] = '\0';
    }

    /* Parse each field separated by comma */
    char *word = strtok(line_copy, ",");
    int field_index = 0;

    while (word != NULL && field_index < 10)
    {
        switch (field_index)
        {
        case 0: /* INSEE code */
            strncpy(city->insee_code, word, 5);
            city->insee_code[5] = '\0';
            break;
        case 1: /* City name */
            strncpy(city->name, word, 63);
            city->name[63] = '\0';
            break;
        case 2: /* Region code */
            strncpy(city->region_code, word, 3);
            city->region_code[3] = '\0';
            break;
        case 3: /* Region name */
            strncpy(city->region_name, word, 63);
            city->region_name[63] = '\0';
            break;
        case 4: /* Department code */
            strncpy(city->dept_code, word, 3);
            city->dept_code[3] = '\0';
            break;
        case 5: /* Department name */
            strncpy(city->dept_name, word, 63);
            city->dept_name[63] = '\0';
            break;
        case 6: /* Postal code */
            strncpy(city->postal_code, word, 5);
            city->postal_code[5] = '\0';
            break;
        case 7: /* Population */
            city->population = atoi(word);
            break;
        case 8: /* Latitude */
            city->latitude = atof(word);
            break;
        case 9: /* Longitude */
            city->longitude = atof(word);
            break;
        }
        word = strtok(NULL, ",");
        field_index++;
    }

    free(line_copy);
    return 0; 
}

/**
 * @brief Loads all cities from a CSV file into memory
 * @param path Path to the CSV file
 * @return Pointer to array of City structures, NULL on failure
 * @note The last City in the array has insee_code[0] = '\0' as terminator
 * @note Must be freed by the caller using free()
 */
City *get_cities_from_csv(char *path)
{
    if (path == NULL)
    {
        fprintf(stderr, "Error: path is NULL\n");
        return NULL;
    }

    /* Get file statistics in one pass */
    int line_count = 0, max_line_length = 0;
    if (getFileStats(path, &line_count, &max_line_length) != 0)
    {
        fprintf(stderr, "Error: Cannot get file stats\n");
        return NULL;
    }

    if (line_count <= 0 || max_line_length <= 0)
    {
        fprintf(stderr, "Error: Invalid file (empty or invalid)\n");
        return NULL;
    }

    /* Allocate array for cities (+1 for null terminator) */
    City *cities = (City *)malloc((line_count + 1) * sizeof(City));
    if (cities == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for cities array\n");
        return NULL;
    }

    /* Allocate buffer for reading lines */
    char *buffer = (char *)malloc((max_line_length + 1) * sizeof(char));
    if (buffer == NULL)
    {
        fprintf(stderr, "Error: Memory allocation failed for buffer\n");
        free(cities);
        return NULL;
    }

    /* Open file */
    FILE *file = open_csv_file(path);
    if (file == NULL)
    {
        fprintf(stderr, "Error: Cannot open file\n");
        free(cities);
        free(buffer);
        return NULL;
    }

    /* Read and parse each line */
    int city_index = 0;
    while (fgets(buffer, max_line_length + 1, file) != NULL && city_index < line_count)
    {
        if (parse_csv_line(buffer, &cities[city_index]) == 0)
        {
            city_index++;
        }
        else
        {
            fprintf(stderr, "Warning: Failed to parse line %d\n", city_index + 1);
        }
    }

    /* Add null terminator at the end */
    cities[city_index].insee_code[0] = '\0';

    /* Cleanup */
    free(buffer);
    close_csv_file(file);

    return cities;
}

/**
 * @brief Prints all cities to stdout
 * @param cities Pointer to array of City structures
 * @param count Number of cities to print (0 for all until null terminator)
 */
void printCities(const City *cities, int count)
{
    if (cities == NULL)
    {
        fprintf(stderr, "Error: cities pointer is NULL\n");
        return;
    }

    int i = 0;
    printf("┌─────────┬──────────────────────────┬──────────────────┬────────────┬────────────┐\n");
    printf("│ INSEE   │ City Name                │ Region           │ Population │ Lat/Lng    │\n");
    printf("├─────────┼──────────────────────────┼──────────────────┼────────────┼────────────┤\n");

    while (cities[i].insee_code[0] != '\0' && (count == 0 || i < count))
    {
        printf("│ %-7s │ %-24s │ %-16s │ %10d │ %.1f/%.1f │\n",
               cities[i].insee_code,
               cities[i].name,
               cities[i].region_name,
               cities[i].population,
               cities[i].latitude,
               cities[i].longitude);
        i++;
    }

    printf("└─────────┴──────────────────────────┴──────────────────┴────────────┴────────────┘\n");
    printf("Total: %d cities displayed\n", i);
}