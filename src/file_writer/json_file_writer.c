#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "file_writer.h"

#define RESULT_FOLDER "../assets/results/"

/**
 * @brief Opens a file stream for writing a file.
 * 
 * If the file exists, it will be overwritten. If it does not exist, 
 * it will be created.
 * 
 * @param filename The path to the file to be created.
 * 
 * @return A pointer to the opened file stream, or NULL if an error occurs.
 */
FILE* open_writing_stream(const char* filename) {
    FILE *f = fopen(filename, "w");
    
    if (f == NULL) {
        printf("Error : Writing stream could not be opened.\n");
    }
    
    return f;
}

/**
 * @brief Gets the current local date and time as a string in format YYYY-MM-DD_HH:MM:SS.
 * 
 * @param buffer A pointer to the char array where the timestamp string will be stored.
 * @param max_size The maximum size of the buffer.
 */
void get_current_datetime(char* buffer, size_t max_size) {
    /* Get the current raw time and convert it to tm structure */
    time_t raw_time;
    time(&raw_time);
    struct tm *time_info;
    time_info = localtime(&raw_time);

    /* Convert the structure tm to a string */
    strftime(buffer, max_size, "%Y-%m-%d_%H:%M:%S", time_info);
}

/**
 * @brief Gets the path were the file has to be save to. Format : assets/results/individual_TIMESTAMP
 * 
 * @param buffer A pointer to the char array where the filepath string will be stored.
 */
void get_filepath(char* buffer) {
    char timestamp[30];
    get_current_datetime(timestamp, sizeof(timestamp));

    strcpy(buffer, RESULT_FOLDER); /* Use strcpy clean the buffer at first insertion in the buffer*/
    strcat(buffer, "individual_");
    strcat(buffer, timestamp);
    strcat(buffer, ".json");
}

/**
 * @brief Write a JSON key in a file, in quote and followed by colon.
 * 
 * @param file The writing stream to the file to write.
 * @param key The key that has to be write.
 */
void json_add_key(FILE* file, const char* key) {
    fprintf(file, "\"%s\": ", key);
}

/**
 * @brief Write a string value in a file, in quote and followed by a comma if needed.
 * 
 * @param file The writing stream to the file to write.
 * @param value The string that has to be written
 * @param has_next If there is following value or not. 1 to add a comma, 0 for not.
 */
void json_add_string(FILE* file, const char* value, int has_next) {
    fprintf(file, "\"%s\"%s\n", value, has_next ? "," : "");
}

/**
 * @brief Write a integer value in a file followed by a comma if needed.
 * 
 * @param file The writing stream to the file to write.
 * @param value The integer that has to be written
 * @param has_next If there is following value or not. 1 to add a comma, 0 for not.
 */
void json_add_int(FILE* file, int value, int has_next) {
    fprintf(file, "%d%s\n", value, has_next ? "," : "");
}

/**
 * @brief Write a double value in a file followed by a comma if needed.
 * 
 * @param file The writing stream to the file to write.
 * @param value The double that has to be written
 * @param has_next If there is following value or not. 1 to add a comma, 0 for not.
 */
void json_add_double(FILE* file, double value, int has_next) {
    fprintf(file, "%.8f%s\n", value, has_next ? "," : "");
}

/**
 * @brief Write a "{" in a file, to start a JSON object
 * 
 * @param file The writing stream to the file to write.
 */
void json_start_object(FILE* file) {
    fprintf(file, "{\n");
}

/**
 * @brief Write a "}" in a file, to end a JSON object, followed by a comma if needed.
 * 
 * @param file The writing stream to the file to write.
 * @param has_next If there is following value or not. 1 to add a comma, 0 for not.
 */
void json_end_object(FILE* file, int has_next) {
    fprintf(file, "}%s\n", has_next ? "," : "");
}

/**
 * @brief Write a "[" in a file, to start a JSON array
 * 
 * @param file The writing stream to the file to write.
 */
void json_start_array(FILE* file) {
    fprintf(file, "[\n");
}

/**
 * @brief Write a "]" in a file, to end a JSON object, followed by a comma if needed.
 * 
 * @param file The writing stream to the file to write.
 * @param has_next If there is following value or not. 1 to add a comma, 0 for not.
 */
void json_end_array(FILE* file, int has_next) {
    fprintf(file, "]%s\n", has_next ? "," : "");
}




void save_individual_and_cities_in_JSON(char* filepath, Individual* individual, City* cities, int cities_size) {
    get_filepath(filepath);

    FILE* file = open_writing_stream(filepath);
    if(file == NULL) {
        return;
    }

    json_start_object(file);
        json_add_key(file, "cities");
        json_start_array(file);
        int i;
        for(i=0; i<cities_size; i++) {
            City city = cities[i];
            json_start_object(file);
                json_add_key(file, "insee_code");
                json_add_string(file, city.insee_code, 1);
                json_add_key(file, "name");
                json_add_string(file, city.name, 1);
                json_add_key(file, "region_code");
                json_add_string(file, city.region_code, 1);
                json_add_key(file, "region_name");
                json_add_string(file, city.region_name, 1);
                json_add_key(file, "dept_code");
                json_add_string(file, city.dept_code, 1);
                json_add_key(file, "dept_name");
                json_add_string(file, city.dept_name, 1);
                json_add_key(file, "postal_code");
                json_add_string(file, city.postal_code, 1);
                json_add_key(file, "population");
                json_add_int(file, city.population, 1);
                json_add_key(file, "latitude");
                json_add_double(file, city.latitude, 1);
                json_add_key(file, "longitude");
                json_add_double(file, city.longitude, 0);
            json_end_object(file, i == cities_size-1 ? 0 : 1); /* If it is the last element, has_next has to be 0 */
        }
        json_end_array(file, 1);
        json_add_key(file, "individual");
        json_start_object(file);
            json_add_key(file, "chru_count");
            json_add_int(file, individual->chru_count, 1);
            json_add_key(file, "isolated_population");
            json_add_int(file, individual->isolated_population, 1);
            json_add_key(file, "score");
            json_add_int(file, individual->score, 1);
            json_add_key(file, "hospitals");
            json_start_array(file);
            for(i=0; i<individual->hospitals_size; i++) {
                Hospital hospital = individual->hospitals[i];
                json_start_object(file);
                    json_add_key(file, "is_chru");
                    json_add_int(file, hospital.is_chru, 1);
                    json_add_key(file, "covered_population");
                    json_add_int(file, hospital.covered_population, 1);
                    json_add_key(file, "bed_count");
                    json_add_int(file, hospital.bed_count, 1);
                    json_add_key(file, "location");
                    json_start_object(file);
                        json_add_key(file, "latitude");
                        json_add_double(file, hospital.location.latitude, 1);
                        json_add_key(file, "longitude");
                        json_add_double(file, hospital.location.longitude, 0);
                    json_end_object(file, 0);
                json_end_object(file, i == individual->hospitals_size-1 ? 0 : 1); /* If it is the last element, has_next has to be 0 */
            }
            json_end_array(file, 0);
        json_end_object(file, 0);
    json_end_object(file, 0);

    fclose(file);

    printf("Individual saved successfully in file : %s\n", filepath);
}