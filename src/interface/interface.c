#include "interface.h"

/* =========================================================
 *  PROJECTION UTILITIES
 *
 *  These functions convert geographic coordinates (latitude,
 *  longitude) into pixel coordinates on the map area.
 *  The bounding box [lat_min, lat_max] x [lon_min, lon_max]
 *  defines the visible area of metropolitan France.
 * ========================================================= */

/**
 * @brief Converts a longitude value to a horizontal pixel coordinate.
 *
 * @param lon     Longitude of the point to project.
 * @param lon_min Minimum longitude in the dataset (western bound).
 * @param lon_max Maximum longitude in the dataset (eastern bound).
 * @return        X pixel coordinate within the map area.
 */
int lon_to_px(double lon, double lon_min, double lon_max)
{
    double ratio = (lon - lon_min) / (lon_max - lon_min);
    return MAP_X + MAP_MARGIN + (int)(ratio * (MAP_W - 2 * MAP_MARGIN));
}

/**
 * @brief Converts a latitude value to a vertical pixel coordinate.
 *
 * @param lat     Latitude of the point to project.
 * @param lat_min Minimum latitude in the dataset (southern bound).
 * @param lat_max Maximum latitude in the dataset (northern bound).
 * @return        Y pixel coordinate within the map area.
 */
int lat_to_py(double lat, double lat_min, double lat_max)
{
    double ratio = (lat_max - lat) / (lat_max - lat_min);
    return MAP_Y + MAP_MARGIN + (int)(ratio * (MAP_H - 2 * MAP_MARGIN));
}


/* =========================================================
 *  MAP DRAWING FUNCTIONS
 * ========================================================= */

/**
 * @brief Draws all French municipalities as small red dots on the map.
 * Cities not covered by any hospital will appear as isolated
 * red points, not hidden under any green or blue circle
 *
 * @param cities      Array of all cities to display.
 * @param city_count  Number of cities in the array.
 * @param lat_min     Southern geographic bound.
 * @param lat_max     Northern geographic bound.
 * @param lon_min     Western geographic bound.
 * @param lon_max     Eastern geographic bound.
 */
void draw_cities(City *cities, int city_count,
                 double lat_min, double lat_max,
                 double lon_min, double lon_max)
{
    int i;
    for (i = 0; i < city_count; i++)
    {
        int px = lon_to_px(cities[i].longitude, lon_min, lon_max);
        int py = lat_to_py(cities[i].latitude, lat_min, lat_max);
        MLV_draw_filled_circle(px, py, CITY_RADIUS, COL_CITY);
    }
}


/* =========================================================
 *  DISPLAY METHODS
 * ========================================================= */

/**
 * @brief Actualise window.
 *
 * @return void.
 */
void actualise_window()
{
    MLV_actualise_window();
}

/**
 * @brief Free window.
 *
 * @return void.
 */
void free_window()
{
    MLV_free_window();
}

/**
 * @brief Split the window in two.
 *
 * @return void.
 */
void split_window()
{
    MLV_Color white = MLV_convert_rgba_to_color(255, 255, 255, 255);
    int size_of_line = 3;
    int x = WIDTH * 0.6;
    MLV_draw_filled_rectangle(x, 0, size_of_line, HEIGHT, white);
}

/**
 * @brief Create window.
 *
 * @return void.
 */
void create_window()
{
    MLV_create_window_with_default_font("Algorithme génétique de répartiitionnement des hopitaux en france mettropoltiatain hors corse", NULL, WIDTH, HEIGHT, "../assets/fonts/FreeMonoBold.ttf", 22);
    split_window();
    MLV_actualise_window();
}

/**
 * 
 * @brief TMP - Draws all cities and refreshes the display for the current generation 
 * 
 * @param cities            Array of all French cities. 
 * @param city_count        Number of cities in the array. 
 * @param lat_min           Southern bound of the bounding box. 
 * @param lat_max           Northern bound of the bounding box. 
 * @param lon_min           Western bound of the bounding box. 
 * @param lon_max           Eastern bound of the bounding box. 
 */
void display(
    City *cities,
    int city_count,
    double lat_min,
    double lat_max,
    double lon_min,
    double lon_max

)
{
    draw_cities(cities, city_count, lat_min, lat_max, lon_min, lon_max);
    MLV_actualise_window();
}
