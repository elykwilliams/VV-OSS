#include <iostream>
#include <fstream>

#include "Base/settings.h"


int main (int argc, char *argv[]){

    for(int i=1; i < argc; ++i) {
        string parameter_file{argv[i]};

        try {
            Settings settings(parameter_file);
            settings.export_settings(parameter_file);
        }

        catch (std::exception &exc) {

            std::cerr << std::endl << std::endl
                      << "----------------------------------------------------"
                      << std::endl;
            std::cerr << "Exception on processing: " << parameter_file << std::endl
                      << exc.what()
                      << "----------------------------------------------------"
                      << std::endl;

            return 1;
        }

    }
}
