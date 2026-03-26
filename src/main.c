#include "interface/interface.h"

int main(int argv, char *argc[])
{

    create_window();

    while (1)
    {
        actualise_window();
    }

    free_window();
    return 0;
}