#ifndef __INTERFACE__
#define __INTERFACE__

#include <MLV/MLV_all.h>
#include "../individual/individual.h"

#define FONT_PATH "../assets/fonts/FreeMonoBold.ttf"
#define FONT_SIZE 18

/* Window dimensions (fixed) */
#define WIDTH  1200
#define HEIGHT 700

/* Map area: left 60% of the window */
#define MAP_X      0
#define MAP_Y      10
#define MAP_W      (int)(WIDTH * 0.60)
#define MAP_H      (HEIGHT - MAP_Y)
#define MAP_MARGIN 20

/* City dot color and size */
#define COL_CITY    MLV_COLOR_RED
#define CITY_RADIUS 1

/* Hospital dot sizes */
#define HOSPITAL_RADIUS 5
#define CHRU_RADIUS     7

/* Hospital and CHRU coverage circle colors */
#define COL_HOSPITAL_RADIUS MLV_rgba(0, 200, 0, 40)
#define COL_CHRU_RADIUS     MLV_rgba(0, 0, 200, 40)

/* Global font loaded once at create_window */
extern MLV_Font *g_font;

/* Refreshes the MLV display buffer */
void actualise_window(void);

/* Frees the MLV window and all its resources */
void free_window(void);

/* Draws the vertical separator between the map and the stats panel */
void split_window(void);

/* Creates and initializes the MLV window */
void create_window(void);

/* Converts a longitude value to a horizontal pixel coordinate */
int lon_to_px(double lon, double lon_min, double lon_max);

/* Converts a latitude value to a vertical pixel coordinate */
int lat_to_py(double lat, double lat_min, double lat_max);

/* Computes the geographic bounding box of a city array */
void compute_bounds(City *cities, int cities_count,
                    double *lat_min, double *lat_max,
                    double *lon_min, double *lon_max);

/* Computes isolated_population for a given individual */
void compute_individual_stats(Individual *individu, City *cities, int city_count);

/* Returns the number of cities not covered by any hospital */
int count_isolated_cities(City *cities, int city_count, Individual *individu);

/* Draws all cities as red dots on the map */
void draw_cities(City *cities, int city_count,
                 double lat_min, double lat_max,
                 double lon_min, double lon_max);

/* Draws all hospitals and CHRU with their coverage radius */
void draw_hospitals(Individual *individu,
                    double lat_min, double lat_max,
                    double lon_min, double lon_max);

/* Draws the legend box in the bottom-left corner of the map */
void draw_legend(void);

/* Draws the scale bar in the bottom-right corner of the map */
void draw_scale(double lon_min, double lon_max);

/* Draws the statistics panel on the right side of the window */
void draw_stats(Individual *individu, int generation,
                City *cities, int city_count, long total_population,
                int isolated_cities);

/* Draws all cities and refreshes the display for the current generation */
void display(City *cities, int city_count,
             double lat_min, double lat_max,
             double lon_min, double lon_max,
             Individual *individu, int generation,
             long total_population);

#endif /* __INTERFACE__ */