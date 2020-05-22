#include <iostream>
#include <deal.II/base/mpi.h>
#include "Base/settings.h"

using namespace dealii;

struct EXCNoParam: std::exception{};

int main (int argc, char *argv[]){

    Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);

    try{

        deallog.depth_console(1);

        string parameter_file{argv[1]};


        std::ifstream infile{parameter_file};
        if (argc != 2 || !infile){
            throw EXCNoParam();
        }
        Settings settings(parameter_file);

    }
    catch (EXCNoParam & exc){

    }
    catch (std::exception &exc) {
        if (Utilities::MPI::this_mpi_process(MPI_COMM_WORLD) == 0) {

            std::cerr << std::endl << std::endl
                      << "----------------------------------------------------"
                      << std::endl;
            std::cerr << "Exception on processing: " << std::endl << exc.what()
                      << std::endl << "Aborting!" << std::endl
                      << "----------------------------------------------------"
                      << std::endl;
        }

        return 1;
    }

    catch (...) {
        if (Utilities::MPI::this_mpi_process(MPI_COMM_WORLD) == 0){
            std::cerr << std::endl << std::endl
                      << "----------------------------------------------------"
                      << std::endl;
            std::cerr << "Unknown exception!" << std::endl << "Aborting!" << std::endl
                      << "----------------------------------------------------"
                      << std::endl;
        }

        return 2;
    }
}
