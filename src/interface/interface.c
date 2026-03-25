#include "interface.h"

/**
 * @brief Create window.
 *
 * @return void.
 */
void create_window()
{
    MLV_create_window_with_default_font("Algorithme génétique de répartiitionnement des hopitaux en france mettropoltiatain hors corse", NULL, WIDTH, HEIGHT, "../assets/fonts/FreeMonoBold.ttf", 22);
    MLV_actualise_window();
}

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