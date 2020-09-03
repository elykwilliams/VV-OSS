//
// Created by kylew on 5/25/20.
//

#include "Base/types.h"
#include "Geometry/mesh_constructors.h"

template<>
void generate_triangulation<TestCase::manufactured>(Triangulation<2>& tria){
    GridGenerator::subdivided_hyper_cube(tria, 1);
}

template<>
void insert_boundary_ids<TestCase::manufactured>(Triangulation<2>& tria){
    for (const auto& cell : tria.active_cell_iterators()) {
        for (unsigned int f = 0; f < GeometryInfo<2>::faces_per_cell; ++f)
            if (cell->at_boundary(f)) {
                cell->face(f)->set_boundary_id(BoundaryType::dirichlet);
            }

    }
}

template
void generate_triangulation<TestCase::manufactured>(Triangulation<2>& tria);

template
void insert_boundary_ids<TestCase::manufactured>(Triangulation<2>& tria);