#define CATCH_CONFIG_RUNNER
#include <mpi.h>
#include "catch.hpp"


TEST_CASE( "test true", "[dummy]" )
{
    REQUIRE( 1 == 1 );
}


int main( int argc, char *argv[] )
{
    MPI_Init(&argc, &argv);

    Catch::Session session;

    int result = session.run();

    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);

    MPI_Finalize();
    if(rank != 0) return 0;

    return result;
}

