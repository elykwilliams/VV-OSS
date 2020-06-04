//
// Created by kylew on 5/22/20.
//

#ifndef NSE_NSESOLVER_H
#define NSE_NSESOLVER_H

#include "Base/settings.h"
#include "Base/fe_handler.h"

#include <deal.II/distributed/tria.h>

//using Trilinos =  LinearAlgebraTrilinos::MPI;

namespace P4est{
    using namespace parallel::distributed;
}


template<int dim>
class NSESolver : public Settings, private MPISettings{
public:
    NSESolver() = delete;
    explicit NSESolver(const Settings& s);



private:
    P4est::Triangulation<dim> triangulation;

    NSEFeHandler<dim> fe_handler;

    void setup_mesh();
};


#endif //NSE_NSESOLVER_H
