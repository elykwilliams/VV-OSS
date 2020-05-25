#include <iostream>
#include <deal.II/base/mpi.h>
#include "Base/settings.h"
#include "Base/NSESolver.h"

using namespace dealii;

struct EXCNoParam: std::exception{};

int main (int argc, char *argv[]){

    Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);

    try{

        deallog.depth_console(1);

        string parameter_file{argv[1]};

        if (argc != 2){
            throw EXCNoParam();
        }
        Settings settings(parameter_file);

        if (settings.get_dim() == 2)
            NSESolver<2> NSE(settings);
        else if (settings.get_dim() == 3)
            NSESolver<2> NSE(settings);
        else
            throw std::runtime_error("Invalid dimension specified in " + parameter_file);

        if (Utilities::MPI::this_mpi_process(MPI_COMM_WORLD) == 0)
            settings.export_settings(parameter_file);

    }
    catch (EXCNoParam & exc){
        if (Utilities::MPI::this_mpi_process(MPI_COMM_WORLD) == 0) {

            std::cerr << std::endl << std::endl
                      << "----------------------------------------------------"
                      << std::endl;
            std::cerr << "No parameter file given."
                      << std::endl << "Aborting!" << std::endl
                      << "----------------------------------------------------"
                      << std::endl;
        }
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
