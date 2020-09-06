//
// Created by kylew on 5/22/20.
//

#ifndef NSE_NSESOLVER_H
#define NSE_NSESOLVER_H

#include "Base/settings.h"
#include "Base/system.h"
#include "Base/fe_handler.h"

#include "Functions/function_constructors.h"

#include <deal.II/distributed/tria.h>
#include <deal.II/fe/fe_system.h>
#include <deal.II/dofs/dof_handler.h>
#include <deal.II/lac/affine_constraints.h>


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
    FESystem<dim> fe;

    ForcingFunction<dim> forcing_function;
    BoundaryConditions<dim> boundary_conditions;
    AnalyticSolution<dim> exact_solution;

    DoFHandler<dim> dof_handler;
    BlockPartitioning<dim> partitioning;
    Solution<dim> solution; // Maybe doesn't need nonlinear iters?

    AffineConstraints<double> constraints; // Maybe move?

    void setup_mesh();
    void setup_system();

    void run();
};





#endif //NSE_NSESOLVER_H
