#include "interface/interface.h"

/*TMP main -> to test interface.c methods */
int main(int argv, char *argc[])
{
    City cities[24] = {
        {"75056", "Paris", "11", "Ile-de-France", "75", "Paris", "75001", 2161000, 48.8566, 2.3522},
        {"13055", "Marseille", "93", "PACA", "13", "Bouches-du-Rhone", "13001", 861635, 43.2965, 5.3698},
        {"69123", "Lyon", "84", "Auvergne-Rhone-Alpes", "69", "Rhone", "69001", 513275, 45.7640, 4.8357},
        {"31555", "Toulouse", "76", "Occitanie", "31", "Haute-Garonne", "31000", 471941, 43.6047, 1.4442},
        {"06088", "Nice", "93", "PACA", "06", "Alpes-Maritimes", "06000", 340017, 43.7102, 7.2620},
        {"44109", "Nantes", "52", "Pays de la Loire", "44", "Loire-Atlantique", "44000", 303382, 47.2184, -1.5536},
        {"67482", "Strasbourg", "44", "Grand Est", "67", "Bas-Rhin", "67000", 280966, 48.5734, 7.7521},
        {"34172", "Montpellier", "76", "Occitanie", "34", "Herault", "34000", 277639, 43.6108, 3.8767},
        {"33063", "Bordeaux", "75", "Nouvelle-Aquitaine", "33", "Gironde", "33000", 254436, 44.8378, -0.5792},
        {"59350", "Lille", "32", "Hauts-de-France", "59", "Nord", "59000", 232741, 50.6292, 3.0573},
        {"35238", "Rennes", "53", "Bretagne", "35", "Ille-et-Vilaine", "35000", 216268, 48.1173, -1.6778},
        {"76540", "Rouen", "28", "Normandie", "76", "Seine-Maritime", "76000", 110169, 49.4432, 1.0993},
        {"38185", "Grenoble", "84", "Auvergne-Rhone-Alpes", "38", "Isere", "38000", 158198, 45.1885, 5.7245},
        {"29019", "Brest", "53", "Bretagne", "29", "Finistere", "29200", 139619, 48.3905, -4.4860},
        {"21231", "Dijon", "27", "Bourgogne-Franche-Comte", "21", "Cote-d-Or", "21000", 156920, 47.3220, 5.0415},
        {"63113", "Clermont-Ferrand", "84", "Auvergne-Rhone-Alpes", "63", "Puy-de-Dome", "63000", 141569, 45.7797, 3.0863},
        {"57463", "Metz", "44", "Grand Est", "57", "Moselle", "57000", 117619, 49.1193, 6.1757},
        {"64445", "Pau", "75", "Nouvelle-Aquitaine", "64", "Pyrenees-Atlantiques", "64000", 77489, 43.2951, -0.3708},
        {"14118", "Caen", "28", "Normandie", "14", "Calvados", "14000", 105512, 49.1829, -0.3707},
        {"49007", "Angers", "52", "Pays de la Loire", "49", "Maine-et-Loire", "49000", 151229, 47.4784, -0.5632},
        {"66056", "Coustouges", "76", "Occitanie", "66", "Pyrenees-Orientales", "66260", 287, 42.3560, -2.6630},
        {"16013", "Aigre", "75", "Nouvelle-Aquitaine", "16", "Charente", "16140", 1400, 45.8970, -0.0100},
        {"67254", "Lauterbourg", "44", "Grand Est", "67", "Bas-Rhin", "67630", 2264, 48.9700, 8.1860},
        {"59095", "Bray-Dunes", "32", "Hauts-de-France", "59", "Nord", "59123", 5000, 51.0730, 2.5300},
    };

    int city_count = 24;
    double lat_min = 42.356; 
    double lat_max = 51.073; 
    double lon_min = -2.663; 
    double lon_max = 8.186;  
    create_window();

    display(cities, city_count, lat_min, lat_max, lon_min, lon_max);
    MLV_wait_keyboard(NULL, NULL, NULL);
    
    /*while (1)
     {
         actualise_window();
     }*/

    free_window();
    return 0;
}