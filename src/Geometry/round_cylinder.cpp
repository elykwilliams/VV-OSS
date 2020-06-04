//
// Created by kylew on 5/25/20.
//

#include "Base/types.h"
#include "Geometry/mesh_constructors.h"

#include <deal.II/grid/grid_generator.h>
#include <deal.II/grid/grid_tools.h>
#include <deal.II/grid/manifold_lib.h>
#include <deal.II/grid/tria.h>

using namespace dealii;

template<>
void generate_triangulation<TestCase::round_cylinder>(Triangulation<2>& tria){
    /* tria1 channel with square portion removed
     *
     * y=0.41 ->   ------------- ... ----
     *            |   ___               |
     * y=0.3 ->   |  |* *|              |
     * y=0.1 ->   |  |_*_|              |
     *            |                     |
     * y=0  ->     ------------- ... ----
     *               ^   ^              ^
     *              x=0.1              x=2.2
     *                  x = 0.3
     */

    Triangulation<2> tria1;

    std::vector< std::vector<double> > spacing(2);

    // Spacing in x direction
    for (int i = 0; i < 4; ++i) spacing[0].push_back(0.1);
    for (int i = 0; i < 3; ++i) spacing[0].push_back(0.6);

    // Spacing in y direction
    for (int i = 0; i < 3; ++i) spacing[1].push_back(0.1);
    spacing[1].push_back(0.11);

    // Mark Cells to be excluded
    Table<2, types::material_id> material_id(spacing[0].size(), spacing[1].size());
    material_id(1,1)=-1;
    material_id(1,2)=-1;
    material_id(2,1)=-1;
    material_id(2,2)=-1;

    GridGenerator::subdivided_hyper_rectangle(tria1, spacing, Point<2>(0, 0), material_id);

    /* tria2 Square with removed round Hole
     * y=0.3 ->   -----------
     *           |    ___    |
     *           |   /   \   |
     * y=0.2 ->  |   | * |   |
     *           |   \___/   |
     *           |           |
     * y=0.1 ->   -----------
     *           ^     ^     ^
     *          x=0.1       x=0.3
     *              x=0.2
     * radius of removed circle = 0.05,
     */
    Triangulation<2> tria2;
    GridGenerator::hyper_cube_with_cylindrical_hole(tria2, 0.05, 0.10);
    tria2.reset_all_manifolds();

    // Move square into place
    const Point<2> cylinder_center(0.2, 0.2);
    GridTools::shift(cylinder_center, tria2);

    // MERGE
    GridGenerator::merge_triangulations(tria1, tria2, tria);

}

template<>
void insert_boundary_ids<TestCase::round_cylinder>(Triangulation<2>& tria){
    constexpr double eps = 1e-5;
    const Point<2> cylinder_center(0.2, 0.2);

    for (const auto& cell : tria.active_cell_iterators()) {
        for (unsigned int f = 0; f < GeometryInfo<2>::faces_per_cell; ++f)
            if (cell->at_boundary(f)) {
                Point<2> face_center = cell->face(f)->center();
                double dx = face_center[0] - cylinder_center[0];
                double dy = face_center[1] - cylinder_center[1];

                if (face_center(0) < eps)
                    cell->face(f)->set_boundary_id(BoundaryType::inflow);
                else if (fabs(2.2 - face_center(0)) < eps)
                    cell->face(f)->set_boundary_id(BoundaryType::outflow);
                else if (dx * dx + dy * dy < 0.05 * 0.05 + eps) {
                    cell->face(f)->set_boundary_id(BoundaryType::bluff);
                    cell->face(f)->set_manifold_id(BoundaryType::bluff);
                } else
                    cell->face(f)->set_boundary_id(BoundaryType::wall);
            }
    }
    static const SphericalManifold<2> manifold_description(cylinder_center);
    tria.set_manifold(BoundaryType::bluff, manifold_description);
}

template
void generate_triangulation<TestCase::round_cylinder>(Triangulation<2>&);

template
void insert_boundary_ids<TestCase::round_cylinder>(Triangulation<2>&);