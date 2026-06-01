#ifndef CONSTANTS_H
#define CONSTANTS_H

/* SOURCES FILES */

#define PATH_TO_CSV_FILE "../assets/data/communes-france-metrople-2025.csv" /* Path to the csv file containing the cities informations */

/* GENETIQUE ADJUSTEMENTS */

#define NB_GENERATIONS 100000 /* The number of generation that the program will process */
#define INDIVIDUALS_SIZE 50   /* The number of individual for each generation */

/* The sum of those has to be equals to the INDIVIDUALS_SIZE */
#define NB_ELITE 5      /* Number of bests indivuals of the previous generation kept in the next generation */
#define NB_CROSSOVER 15 /* Number of crossover, between two individuals of the previous generation, to add in the next generation */
#define NB_MUTATION 20  /* Number of mutation of the best individuals of the previous generation to add in the next generation */
#define NB_RANDOM 10    /* Number of new random individual to add in the next generation */

/* Don't reduce to much the chances or an Individual could have no hospital, which will conduct to indesirable effects */
#define CHANCES_MIN_TO_PLACE_HOSPITAL 5 /* Percentage minimal of chances to associate an hospital to a city when creating randomly, on 100 */
#define CHANCES_MAX_TO_PLACE_HOSPITAL 8 /* Percentage maximal of chances to associate an hospital to a city when creating randomly, on 100 */

#define MUTATION_CHANCES_TO_ADD_NEW 2 /* Percentage to add a new hopsital for each existing hospital, on 100 */
#define MUTATION_CHANCES_TO_REMOVE 2  /* Percentage to remove the hopsital for each existing hospital, on 100 */

#define PENALTY_ISOLATED_POP 1 /* The penalty given for each isolated people in the formula to compute fitness (default is 1) */
#define PENALTY_HOSPITAL 5000  /* The penalty given for each hospital in the formula to compute the fitness (default is 5000) */
#define BONUS_CHRU 4000        /* The bonus given for each CHRU in the formula to compute the fitness (default is 4000) */

#define CHRU_MIN_POPULATION 80000 /* The number of a city population so the implented hospital is considered as a CHRU*/
#define BED_FOR_POPULATION 5.4    /* Number of bed needed for 1000 people covered by an hospital*/

/* OTHER CONSTANTS */

#define M_PI 3.1415
#define EARTH_RADIUS_KM 6371.0

#endif