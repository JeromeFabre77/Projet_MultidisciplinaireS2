#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "../city/city.h"

/*Evite de tomber sur le champ d'en dessous en cas de malformation du fichier*/
#define CSV_FIELD_COUNT 10
/* 4096 pour être safe et afficher 4096 caractères possibles avant de connaitre la taille de la longest line
et redimensionner le buffer de lecture  */
#define LINE_READ_CHUNK 4096

/**
 * @brief Ouvre un fichier en mode lecture avec verification d'erreurs
 * @param path Path to the file to open
 * @return FILE pointer if successful, NULL on failure
 */
FILE *open_file(const char *path) {
    FILE *file;

    /* Verifie que le chemin est valide avant d'ouvrir le fichier. */
    if (path == NULL) {
        fprintf(stderr, "Error: path is NULL\n");
        return NULL;
    }

    /* Ouvre le fichier en lecture seule. */
    file = fopen(path, "r");
    if (file == NULL) {
        fprintf(stderr, "Error: Cannot open file '%s'\n", path);
        return NULL;
    }

    return file;
}

/**
 * @brief Ferme un fichier avec verification d'erreurs
 * @param file FILE pointer to close
 * @return 0 on success, EOF on failure
 */
int close_file(FILE *file) {
    /* Un pointeur NULL n'est pas fatal ici: on considere que rien n'est a fermer. */
    if (file == NULL) {
        fprintf(stderr, "Warning: Attempting to close NULL file pointer\n");
        return 0;
    }

    if (fclose(file) == EOF) {
        fprintf(stderr, "Error: Failed to close file\n");
        return EOF;
    }

    return 0;
}

/**
 * @brief Calcule les statistiques du fichier en un seul passage
 * @param path Path to the CSV file
 * @param line_count Pointer to store the line count
 * @param max_length Pointer to store the max line length
 * @return 0 on success, -1 on failure
 */
int get_file_stats(const char *path, int *line_count, int *max_length) {
    FILE *file;
    char buffer[LINE_READ_CHUNK];
    int len;

    /* Validation defensive des pointeurs de sortie. */
    if (line_count == NULL || max_length == NULL) {
        fprintf(stderr, "Error: NULL pointer in get_file_stats\n");
        return -1;
    }

    file = open_file(path);
    if (file == NULL) {
        return -1;
    }

    *line_count = 0;
    *max_length = 0;

    /* Parcours complet pour compter les lignes et trouver la plus longue. */
    while (fgets(buffer, sizeof(buffer), file) != NULL) {
        (*line_count)++;
        len = (int)strlen(buffer);
        if (len > *max_length) {
            *max_length = len;
        }
    }

    if (close_file(file) == EOF) {
        return -1;
    }

    return 0;
}

/**
 * @brief Parse une ligne CSV et remplit une structure City
 * @param line The CSV line to parse (format: insee,name,region_code,region_name,dept_code,dept_name,postal_code,population,latitude,longitude)
 * @param city Pointer to City structure to fill
 * @return 0 on success, -1 on failure
 */
int parse_csv_line(const char *line, City *city) {
    char *line_copy;
    size_t len;
    char *word;
    int field_index;

    if (line == NULL || city == NULL) {
        fprintf(stderr, "Error: NULL pointer in parse_csv_line\n");
        return -1;
    }

    /* Copie de travail: strtok modifie la chaine d'entree. */
    line_copy = (char *)malloc((strlen(line) + 1) * sizeof(char));
    if (line_copy == NULL) {
        fprintf(stderr, "Error: Memory allocation failed\n");
        return -1;
    }

    strcpy(line_copy, line);

    /* Supprime le saut de ligne ajoute par fgets. */
    len = strlen(line_copy);
    if (len > 0 && line_copy[len - 1] == '\n') {
        line_copy[len - 1] = '\0';
    }

    /* Decoupage champ par champ. */
    word = strtok(line_copy, ",");
    field_index = 0;

    while (word != NULL && field_index < CSV_FIELD_COUNT) {
        switch (field_index) {
        case 0:
            strncpy(city->insee_code, word, INSEE_CODE_BYTES);
            city->insee_code[INSEE_CODE_BYTES] = '\0';
            break;
        case 1:
            strncpy(city->name, word, CITY_NAME_BYTES);
            city->name[CITY_NAME_BYTES] = '\0';
            break;
        case 2:
            strncpy(city->region_code, word, REGION_CODE_BYTES);
            city->region_code[REGION_CODE_BYTES] = '\0';
            break;
        case 3:
            strncpy(city->region_name, word, REGION_NAME_BYTES);
            city->region_name[REGION_NAME_BYTES] = '\0';
            break;
        case 4:
            strncpy(city->dept_code, word, DEPT_CODE_BYTES);
            city->dept_code[DEPT_CODE_BYTES] = '\0';
            break;
        case 5:
            strncpy(city->dept_name, word, DEPT_NAME_BYTES);
            city->dept_name[DEPT_NAME_BYTES] = '\0';
            break;
        case 6:
            strncpy(city->postal_code, word, POSTAL_CODE_BYTES);
            city->postal_code[POSTAL_CODE_BYTES] = '\0';
            break;
        case 7:
            city->population = atoi(word);
            break;
        case 8:
            city->latitude = atof(word);
            break;
        case 9:
            city->longitude = atof(word);
            break;
        }

        word = strtok(NULL, ",");
        field_index++;
    }

    /* On exige exactement le nombre de champs attendu. */
    if (field_index != CSV_FIELD_COUNT) {
        fprintf(stderr,
                "Error: Invalid CSV format, expected %d fields and got %d\n",
                CSV_FIELD_COUNT,
                field_index);
        free(line_copy);
        return -1;
    }

    free(line_copy);
    return 0;
}

/**
 * @brief Loads all cities from a CSV file into memory
 * @param path Path to the CSV file
 * @param cities Output pointer receiving allocated City array
 * @return Number of parsed cities on success, -1 on failure
 */
int get_cities_from_csv(const char *path, City **cities) {
    int line_count;
    int max_line_length;
    char *buffer;
    FILE *file;
    int city_index;
    City *shrunk_cities;

    /* Validation d'entree et initialisation de la sortie. */
    if (path == NULL || cities == NULL) {
        fprintf(stderr, "Error: path or cities is NULL\n");
        return -1;
    }

    *cities = NULL;

    /* Premiere passe: determine la capacite necessaire. */
    line_count = 0;
    max_line_length = 0;
    if (get_file_stats(path, &line_count, &max_line_length) != 0) {
        fprintf(stderr, "Error: Cannot get file stats\n");
        return -1;
    }

    if (line_count <= 0 || max_line_length <= 0) {
        fprintf(stderr, "Error: Invalid file (empty or invalid)\n");
        return -1;
    }

    /* Allocation du tableau de villes. */
    *cities = city_resize_array(NULL, line_count);
    if (*cities == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for cities array\n");
        return -1;
    }

    /* Buffer dynamique adapte a la ligne la plus longue. */
    buffer = (char *)malloc((max_line_length + 1) * sizeof(char));
    if (buffer == NULL) {
        fprintf(stderr, "Error: Memory allocation failed for buffer\n");
        free(*cities);
        *cities = NULL;
        return -1;
    }

    file = open_file(path);
    if (file == NULL) {
        free(*cities);
        *cities = NULL;
        free(buffer);
        return -1;
    }

    /* Deuxieme passe: lecture et parsing de chaque ligne. */
    city_index = 0;
    while (fgets(buffer, max_line_length + 1, file) != NULL && city_index < line_count) {
        if (parse_csv_line(buffer, &(*cities)[city_index]) != 0) {
            fprintf(stderr, "Error: Failed to parse line %d\n", city_index + 1);
            free(*cities);
            *cities = NULL;
            free(buffer);
            close_file(file);
            return -1;
        }
        city_index++;
    }

    /* Reallocation pour ne garder que les villes effectivement parsees. */
    shrunk_cities = city_resize_array(*cities, city_index);
    if (shrunk_cities != NULL || city_index == 0) {
        *cities = shrunk_cities;
    }

    free(buffer);

    if (close_file(file) == EOF) {
        free(*cities);
        *cities = NULL;
        return -1;
    }

    return city_index;
}
