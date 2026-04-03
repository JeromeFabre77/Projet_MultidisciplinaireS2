#include "utils.h"
#define M_PI 3.1415

/**
 * @brief Convert degres to radiants.
 *
 * @param deg is a double.
 *
 * @return A radiant which is a double.
 */
double deg_to_rad(double deg)
{
    return deg * M_PI / 180.0;
}