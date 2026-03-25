#ifndef __FILE_WRITER__
#define __FILE_WRITER__


typedef struct {
  int a;
} Individual;


/**
 * @brief Save the datas of the individual in a JSON file located to the file path returned. 
 * According to the structure Individual, the data is written with the following format :
 *
 * {
 *   "key" : "value"
 * }

 * @param individual The individual to save. 
 * @param other Something else.

 * @return The path to the JSON file where the individual has been written.
 */
char* save_individual_in_JSON(Individual* individual);



#endif