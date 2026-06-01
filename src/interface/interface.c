#define _DEFAULT_SOURCE
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

int count_isolated_cities(City *cities, int city_count, Individual *individu)
{
    int i, j;
    int isolated = 0;

    /* Boolean array: 0 = isolated, 1 = covered */
    int *covered = calloc(city_count, sizeof(int));

    /* Mark all cities covered by at least one hospital */
    for (i = 0; i < individu->hospitals_size; i++)
        for (j = 0; j < individu->hospitals[i]->cities_size; j++)
            covered[individu->hospitals[i]->cities[j] - cities] = 1;

    /* Count isolated */
    for (i = 0; i < city_count; i++)
        if (!covered[i])
            isolated++;

    free(covered);
    return isolated;
}

void compute_individual_stats(Individual *individu, City *cities, int city_count)
{
    int i, j, idx;
    int *covered = calloc(city_count, sizeof(int));
    long isolated_pop = 0;

    for (i = 0; i < individu->hospitals_size; i++)
        for (j = 0; j < individu->hospitals[i]->cities_size; j++)
        {
            idx = individu->hospitals[i]->cities[j] - cities;
            if (idx >= 0 && idx < city_count)
                covered[idx] = 1;
        }

    for (i = 0; i < city_count; i++)
        if (!covered[i])
            isolated_pop += cities[i].population;

    individu->isolated_population = (int)isolated_pop;
    free(covered);
}

/* =========================================================
 *  MAP DRAWING FUNCTIONS
 * ========================================================= */

void compute_bounds(City *cities, int cities_count, double *lat_min, double *lat_max, double *lon_min, double *lon_max)
{
    int i;
    for (i = 0; i < cities_count; i++)
    {
        if (cities[i].latitude < *lat_min)
            *lat_min = cities[i].latitude;
        if (cities[i].latitude > *lat_max)
            *lat_max = cities[i].latitude;
        if (cities[i].longitude < *lon_min)
            *lon_min = cities[i].longitude;
        if (cities[i].longitude > *lon_max)
            *lon_max = cities[i].longitude;
    }
}
/**
 * @brief Draws all French municipalities as small red dots on the map.
 * Cities not covered by any hospital will appear as isolated
 * red points, not hidden under any green or blue circle
 *
 * @param cities      Array of all cities to display.
 * @param city_count  Number of cities in the array.
 * @param lat_min     Southern geographic bound.
 * @param lat_max     Northern geographic bound.
 * @param lon_myin     Western geographic bound.
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
void draw_hospitals(Individual *individu,
                    double lat_min, double lat_max,
                    double lon_min, double lon_max)
{
    int i;
    int px, py, radius_px;

    radius_px = (int)(COVERAGE_RADIUS_KM / 75.0 * (MAP_W - 2 * MAP_MARGIN) / (lon_max - lon_min) * 0.6);
    /*printf("radius_px = %d\n", radius_px);*/
    /* Pass 1: regular hospitals (green) */
    for (i = 0; i < individu->hospitals_size; i++)
    {
        if (individu->hospitals[i]->is_chru) continue;
        px = lon_to_px(individu->hospitals[i]->location->longitude, lon_min, lon_max);
        py = lat_to_py(individu->hospitals[i]->location->latitude,  lat_min, lat_max);
        MLV_draw_filled_circle(px, py, radius_px, COL_HOSPITAL_RADIUS);
        MLV_draw_circle(px, py, radius_px, MLV_COLOR_GREEN);
        MLV_draw_filled_circle(px, py, HOSPITAL_RADIUS, MLV_COLOR_GREEN);
    }

    /* Pass 2: CHRU on top (blue) */
    for (i = 0; i < individu->hospitals_size; i++)
    {
        if (!individu->hospitals[i]->is_chru) continue;
        px = lon_to_px(individu->hospitals[i]->location->longitude, lon_min, lon_max);
        py = lat_to_py(individu->hospitals[i]->location->latitude,  lat_min, lat_max);
        MLV_draw_filled_circle(px, py, radius_px, COL_CHRU_RADIUS);
        MLV_draw_circle(px, py, radius_px, MLV_COLOR_BLUE);
        MLV_draw_filled_circle(px, py, CHRU_RADIUS, MLV_COLOR_BLUE);
    }
}
void draw_legend(void)
{
    int i;
    int box_x = MAP_X + MAP_MARGIN;
    int box_y = HEIGHT - (int)(HEIGHT * 0.20);
    int box_w = (int)(MAP_W * 0.16);
    int box_h = (int)(HEIGHT * 0.18);
    int title_y = box_y + (int)(box_h * 0.05);
    int first_dot_y = box_y + (int)(box_h * 0.35);
    int line_height = (int)(box_h * 0.28);
    int dot_radius = 4;
    int dot_x = box_x + (int)(box_w * 0.08);
    int text_x = box_x + (int)(box_w * 0.20);
    int text_offset = (int)(line_height * 0.35);

    const char *labels[] = {"Commune", "Hopital", "CHRU"};
    MLV_Color colors[] = {MLV_COLOR_RED, MLV_COLOR_GREEN, MLV_COLOR_BLUE};

    MLV_draw_filled_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_BLACK);
    MLV_draw_rectangle(box_x, box_y, box_w, box_h, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(box_x + (int)(box_w * 0.08), title_y, "Legende", g_font, MLV_COLOR_WHITE);

    for (i = 0; i < 3; i++)
    {
        int y = first_dot_y + i * line_height;
        MLV_draw_filled_circle(dot_x, y, dot_radius, colors[i]);
        MLV_draw_text_with_font(text_x, y - text_offset, labels[i], g_font, MLV_COLOR_WHITE);
    }
}

void draw_scale(double lon_min, double lon_max)
{
    int padding = (int)(MAP_W * 0.008);
    int tick_h = (int)(HEIGHT * 0.025);
    int tick_w = 2;
    int bar_h = (int)(HEIGHT * 0.006);
    int tick_offset = (int)(tick_h * 0.35);
    int label_w = 70;

    int scale_px = (int)(100.0 / 75.0 * (MAP_W - 2 * MAP_MARGIN) / (lon_max - lon_min));

    int sx = MAP_W - MAP_MARGIN - scale_px;
    int sy = HEIGHT - (int)(HEIGHT * 0.04);

    MLV_draw_filled_rectangle(sx - padding, sy - tick_h, scale_px + 2 * padding, tick_h + bar_h + padding, MLV_COLOR_BLACK);
    MLV_draw_filled_rectangle(sx, sy, scale_px, bar_h, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(sx, sy - tick_offset, tick_w, tick_h, MLV_COLOR_WHITE);
    MLV_draw_filled_rectangle(sx + scale_px - tick_w, sy - tick_offset, tick_w, tick_h, MLV_COLOR_WHITE);
    MLV_draw_text_with_font(sx + scale_px / 2 - label_w / 2, sy - tick_h, "100 km", g_font, MLV_COLOR_WHITE);

}

/* =========================================================
 *  PANNEL STATS FUNCTIONS
 * ========================================================= */

static int draw_title(int x, int y, int line_h, const char *text)
{
    MLV_draw_text_with_font(x, y, text,g_font, MLV_COLOR_YELLOW);
    return y + line_h;
}

static int draw_stat(int x, int y, int line_h, int indent, const char *text)
{
    MLV_draw_text_with_font(x + indent, y, text,g_font, MLV_COLOR_WHITE);
    return y + line_h;
}

static int draw_separator(int x, int y, int line_h, int panel_w, int thickness)
{
    MLV_draw_filled_rectangle(x, y, panel_w, thickness, MLV_COLOR_GRAY);
    return y + line_h / 2;
}

void draw_stats(Individual *individu, int generation, int city_count, long total_population, int isolated_cities)
{

    char buf[64];

    int panel_x = (int)(WIDTH * STATS_PANEL_WEIGHT) + MAP_MARGIN;
    int panel_w = (int)(WIDTH * 0.38);
    int line_h = (int)(HEIGHT * 0.045);
    int indent = (int)(WIDTH * 0.02);
    int y = MAP_MARGIN;

    /* --- FITNESS --- */
    snprintf(buf, sizeof(buf), "Fitness    : %d", individu->score);
    y = draw_title(panel_x, y, line_h, buf);
    snprintf(buf, sizeof(buf), "Generation n°%d", generation);
    y = draw_stat(panel_x, y, line_h, indent, buf);
    y = draw_separator(panel_x, y, line_h, panel_w, 2);

    /* --- CPT --- */
    y = draw_title(panel_x, y, line_h, "[Hopitaux]");
    snprintf(buf, sizeof(buf), "Nb hopitaux : %d", individu->hospitals_size);
    y = draw_stat(panel_x, y, line_h, indent, buf);
    snprintf(buf, sizeof(buf), "Nb CHRU     : %d", individu->chru_count);
    y = draw_stat(panel_x, y, line_h, indent, buf);
    y = draw_separator(panel_x, y, line_h, panel_w, 1);

    /* --- POP --- */
    y = draw_title(panel_x, y, line_h, "[Population]");
    snprintf(buf, sizeof(buf), "Pop. totale   : %ld", total_population);

    y = draw_stat(panel_x, y, line_h, indent, buf);
    snprintf(buf, sizeof(buf), "Pop. eloignee : %d", individu->isolated_population);
    y = draw_stat(panel_x, y, line_h, indent, buf);
    snprintf(buf, sizeof(buf), "Pct non couv. : %.2f",
             100.0 * individu->isolated_population / total_population);
    y = draw_stat(panel_x, y, line_h, indent, buf);
    y = draw_separator(panel_x, y, line_h, panel_w, 1);

    /* --- CITIES --- */
    y = draw_title(panel_x, y, line_h, "[Villes]");
    snprintf(buf, sizeof(buf), "Nb villes      : %d", city_count);
    y = draw_stat(panel_x, y, line_h, indent, buf);
    snprintf(buf, sizeof(buf), "Nb isol.       : %d", isolated_cities);
    y = draw_stat(panel_x, y, line_h, indent, buf);
    snprintf(buf, sizeof(buf), "Pct isol.      : %.2f",
             100.0 * isolated_cities / city_count);
    y = draw_stat(panel_x, y, line_h, indent, buf);

    (void)y;
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
    MLV_free_font(g_font);
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
    int x = WIDTH * STATS_PANEL_WEIGHT;
    MLV_draw_filled_rectangle(x, 0, size_of_line, HEIGHT, white);
}

/**
 * @brief Create window.
 *
 * @return void.
 */

MLV_Font *g_font = NULL;

void create_window()
{
    MLV_create_window_with_default_font("Algorithme génétique de répartitionnement des hopitaux en france metropolitaine hors corse", NULL, WIDTH, HEIGHT, "../assets/fonts/FreeMonoBold.ttf", 22);
    g_font = MLV_load_font(FONT_PATH,FONT_SIZE);
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
    double lon_max,
    Individual *individu,
    int generation,
    long total_population)
{
    int isolated_cities = count_isolated_cities(cities, city_count, individu);

    MLV_clear_window(MLV_COLOR_BLACK);
    draw_cities(cities, city_count, lat_min, lat_max, lon_min, lon_max);
    draw_hospitals(individu, lat_min, lat_max, lon_min, lon_max);
    draw_legend();
    draw_scale(lon_min, lon_max);
    draw_stats(individu, generation,city_count, total_population, isolated_cities);
    MLV_actualise_window();
}