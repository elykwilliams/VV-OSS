#define CATCH_CONFIG_RUNNER
// #include "testing.h"
#include <deal.II/base/mpi.h>
#include "catch.hpp"
#include <iostream>
TEST_CASE( "test1", "[factorial]" )
{
	int rank = -1;
  	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  	if(rank == 0)	
		REQUIRE( rank == dealii::Utilities::MPI::this_mpi_process(MPI_COMM_WORLD) );
	else
    //	REQUIRE( rank != dealii::Utilities::MPI::this_mpi_process(MPI_COMM_WORLD) );
}


TEST_CASE("test2", "[notfactorial]")
{
	REQUIRE(2 != 1);
}

int main( int argc, char *argv[] )
{
  using namespace dealii;
  Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv, 1);

  Catch::Session session;
  session.applyCommandLine(argc, argv);
  int result = session.run();
  return result;
}

