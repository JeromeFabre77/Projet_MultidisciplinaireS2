#include "file_writer/file_writer.h"

int main(int argv, char* argc[]){

    Individual oui = {1};

    save_individual_in_JSON(&oui);
    return 0;
}