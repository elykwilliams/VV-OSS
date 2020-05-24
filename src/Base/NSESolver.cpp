//
// Created by kylew on 5/22/20.
//

#include "NSESolver.h"

template<int dim>
NSESolver<dim>::NSESolver(const Settings& s):
    Settings(s),
    triangulation(mpi_comm)
{

}

template class NSESolver<2>;
template class NSESolver<3>;