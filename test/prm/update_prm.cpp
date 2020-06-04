#include <iostream>
#include <string>

#include "Base/settings.h"

using std::string;

int main (int argc, char *argv[]){

    for(int i=1; i < argc; ++i) {
        string parameter_file{argv[i]};
        try {
            Settings settings(parameter_file);
      		std::cout << "Updating " << parameter_file << "\n";
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
