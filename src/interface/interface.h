#ifndef __INTERFACE__
#define __INTERFACE__
#include <MLV/MLV_all.h>
#include "../individual/individual.h"*

/* Window dimensions (fixed) */
#define WIDTH 1200
#define HEIGHT 700

/* Map area: left 60% of the window */
#define MAP_X        0
#define MAP_Y        40        
#define MAP_W        (int)(WIDTH * 0.60)
#define MAP_H        (HEIGHT - MAP_Y)
#define MAP_MARGIN   20

/* City dot color and size */
#define COL_CITY         MLV_COLOR_RED         
#define CITY_RADIUS      2

/* Hospital dot sizes */
#define HOSPITAL_RADIUS  5
#define CHRU_RADIUS      7

/* Refreshes the MLV display buffer */
void actualise_window();

/* Frees the MLV window and all its resources */
void free_window();

/* Draws the vertical separator between the map and the stats panel */
void split_window();

/* Creates and initializes the MLV window */
void create_window();

/* Converts a longitude value to a horizontal pixel coordinate */
int lon_to_px(double lon, double lon_min, double lon_max);

/* Converts a latitude value to a vertical pixel coordinate */
int lat_to_py(double lat, double lat_min, double lat_max);

/* Draws all cities as red dots on the map */
void draw_cities(City* cities, int city_count,
                 double lat_min, double lat_max,
                 double lon_min, double lon_max);

/* Draws all cities and refreshes the display for the current generation */
void display(City* cities, int city_count,
             double lat_min, double lat_max,
             double lon_min, double lon_max);

#endif /* __INTERFACE__ */