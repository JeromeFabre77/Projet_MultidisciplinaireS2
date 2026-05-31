#ifndef __INTERFACE__
#define __INTERFACE__

#include <MLV/MLV_all.h>
#include "../individual/individual.h"
#include <stdio.h>
#include <stdlib.h>

#define FONT_PATH "../assets/fonts/FreeMonoBold.ttf"
#define FONT_SIZE 18

/* Window dimensions (fixed) */
#define WIDTH  1200
#define HEIGHT 700
#define STATS_PANEL_WEIGHT 0.6

/* Map area: left 60% of the window */
#define MAP_X      0
#define MAP_Y      10
#define MAP_W      (int)(WIDTH * STATS_PANEL_WEIGHT)
#define MAP_H      (HEIGHT - MAP_Y)
#define MAP_MARGIN 20

/* City dot color and size */
#define COL_CITY    MLV_COLOR_RED
#define CITY_RADIUS 1

/* Hospital dot sizes */
#define HOSPITAL_RADIUS 4
#define CHRU_RADIUS     4

/* Hospital and CHRU coverage circle colors */
#define COL_HOSPITAL_RADIUS MLV_rgba(0, 80, 0, 255)
#define COL_CHRU_RADIUS     MLV_rgba(0, 0, 80, 25)



/* Global font loaded once at create_window */
extern MLV_Font *g_font;

/* Creates and initializes the MLV window */
void create_window(void);

/* Computes the geographic bounding box of a city array */
void compute_bounds(City *cities, int cities_count,
                    double *lat_min, double *lat_max,
                    double *lon_min, double *lon_max);

/* Draws all cities and refreshes the display for the current generation */
void display(City *cities, int city_count,
             double lat_min, double lat_max,
             double lon_min, double lon_max,
             Individual *individu, int generation,
             long total_population);

#endif /* __INTERFACE__ */