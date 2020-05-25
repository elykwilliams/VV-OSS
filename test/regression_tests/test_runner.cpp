//
// Created by kylew on 5/21/20.
//
#define CATCH_CONFIG_RUNNER
#include <string>
#include <deal.II/base/mpi.h>

#include "catch.hpp"

using std::string;
namespace MPI{
    using namespace dealii::Utilities::MPI;
}

string filename;

int main (int argc, char *argv[]){

    MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);
    filename = string(argv[1]);

    Catch::Session session;
    int result = session.run();

    if(MPI::this_mpi_process(MPI_COMM_WORLD) == 0)
        return result;
    return 0;
}
