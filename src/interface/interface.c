#include "interface.h"

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