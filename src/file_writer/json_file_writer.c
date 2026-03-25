#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#include "file_writer.h"

#define RESULT_FOLDER ../../assets/results


/**
 * @brief Opens a file stream for writing a JSON file.
 * 
 * If the file exists, it will be overwritten. If it does not exist, 
 * it will be created.
 * 
 * @param filename The path to the file to be created.
 * 
 * @return A pointer to the opened file stream, or NULL if an error occurs.
 */
FILE* open_writing_stream(const char *filename) {
    FILE *f = fopen(filename, "w");
    
    if (f == NULL) {
        printf("Error : Writing stream could not be opened ");
    }
    
    return f;
}

/**
 * @brief Gets the current local date and time as a string in format YYYY-MM-DD HH:MM:SS.
 * 
 * @param buffer A pointer to the char array where the string will be stored.
 * @param max_size The maximum size of the buffer.
 */
void get_current_datetime(char *buffer, size_t max_size) {
    time_t raw_time;
    struct tm *time_info;

    /* Get the current raw time and convert it to local time structure */
    time(&raw_time);
    time_info = localtime(&raw_time);

    strftime(buffer, max_size, "%Y-%m-%d %H:%M:%S", time_info);
}

char* save_individual_in_JSON(Individual* individual) {
  char timestamp[30];
  get_current_datetime(timestamp, sizeof(timestamp));

  printf(open_writing_stream(strcat(timestamp, timestamp)));

  /*FILE* file = open_writing_stream(strcat(RESULT_FOLDER, timestamp));*/



  char* yes;
  return yes;
}
