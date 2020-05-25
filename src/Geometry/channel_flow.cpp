//
// Created by kylew on 5/25/20.
//

#include "Base/types.h"
#include "Geometry/mesh_constructors.h"

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/tria.h>

using namespace dealii;

template<>
void generate_triangulation<TestCase::channel_flow>(Triangulation<2>& tria){
    /* tria1 channel with square portion removed
     *
     * y=0.41 ->   ---------------------
     *            |                     |
     *            |                     |
     *            |                     |
     *            |                     |
     * y=0  ->     ---------------------
     *            ^                     ^
     *           x=0                   x=2.2
     *
     */

    GridGenerator::hyper_rectangle(tria, Point<2>(0, 0), Point<2>(2.2, 0.41));

}

template<>
void insert_boundary_ids<TestCase::channel_flow>(Triangulation<2>& tria){
    constexpr double eps = 1e-5;

    for (const auto& cell : tria.active_cell_iterators())
        for (unsigned int f = 0; f < GeometryInfo<2>::faces_per_cell; ++f)
            if (cell->at_boundary(f)) {
                Point<2> face_center = cell->face(f)->center();
                if (face_center(0) < eps)
                    cell->face(f)->set_boundary_id(BoundaryType::inflow);
                else if (fabs(2.2 - face_center(0)) < eps)
                    cell->face(f)->set_boundary_id(BoundaryType::outflow);
                else
                    cell->face(f)->set_boundary_id(BoundaryType::wall);
            }
}

template
void generate_triangulation<TestCase::channel_flow>(Triangulation<2>& tria);

template
void insert_boundary_ids<TestCase::channel_flow>(Triangulation<2>& tria);