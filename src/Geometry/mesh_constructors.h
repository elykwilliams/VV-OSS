//
// Created by kylew on 5/25/20.
//

#ifndef NSE_MESH_CONSTRUCTORS_H
#define NSE_MESH_CONSTRUCTORS_H

#include "Base/types.h"
#include <deal.II/grid/grid_generator.h>

using namespace dealii;

namespace P4est{
    using namespace parallel::distributed;
}


template<TestCase name>
void generate_triangulation(Triangulation<2>&){}

template<TestCase name>
void generate_triangulation(Triangulation<3>&){}

template<TestCase name>
void insert_boundary_ids(Triangulation<2>&){}

template<TestCase name>
void insert_boundary_ids(Triangulation<3>&){}

#endif //NSE_MESH_CONSTRUCTORS_H
