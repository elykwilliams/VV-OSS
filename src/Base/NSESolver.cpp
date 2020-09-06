//
// Created by kylew on 5/22/20.
//

#include "NSESolver.h"
#include <sys/stat.h>

template<int dim>
NSESolver<dim>::NSESolver(const Settings& s):
    Settings(s),
    triangulation(mpi_comm),
    fe_handler(s.feSettings),
    fe(fe_handler.fes(), fe_handler.multiplicities()),
    forcing_function(s.testcaseSettings, fe_handler),
    boundary_conditions(s.testcaseSettings, fe_handler),
    exact_solution(s.testcaseSettings, fe_handler),
    dof_handler(triangulation)
{
    struct stat sb{};
    if (stat(generalSettings.output_dir.c_str(), &sb) != 0)
        mkdir(generalSettings.output_dir.c_str(), 0777);

    setup_mesh();
    setup_system();
}

#include <deal.II/grid/grid_out.h>
#include "Geometry/mesh_constructors.h"
template<int dim>
void NSESolver<dim>::setup_mesh(){

    switch(testcaseSettings.name){
        case TestCase::manufactured:
            generate_triangulation<TestCase::manufactured>(triangulation);
            insert_boundary_ids<TestCase::manufactured>(triangulation);
            break;
        case TestCase::channel_flow:
            generate_triangulation<TestCase::channel_flow>(triangulation);
            insert_boundary_ids<TestCase::channel_flow>(triangulation);
            break;
        case TestCase::round_cylinder:
            generate_triangulation<TestCase::round_cylinder>(triangulation);
            insert_boundary_ids<TestCase::round_cylinder>(triangulation);
            break;
    }

    triangulation.refine_global(meshSettings.initial_refinements);

    if (meshSettings.output_mesh)
    {
        GridOut out;
        std::string filename = generalSettings.output_dir + filename_base + "-grid";
        out.write_mesh_per_processor_as_vtu(triangulation, filename);
        pcout << "Mesh saved to file " << filename << ".pvtu" << std::endl;
    }
}

#include <deal.II/numerics/vector_tools.h>

template<int dim>
void NSESolver<dim>::setup_system(){
    dof_handler.distribute_dofs(fe);
    partitioning = BlockPartitioning(dof_handler, fe_handler);
    solution = Solution(partitioning, mpi_comm);

    constraints.reinit(partitioning.locally_relevant_dofs);

    DoFTools::make_hanging_node_constraints(dof_handler, constraints);

    VectorTools::interpolate_boundary_values(dof_handler,
                                             boundary_conditions.velocity(),
                                             constraints,
                                             fe_handler.component_mask(VariableName::Velocity));

    // TODO: Need to update vorticity boundary condition with velocity solution
    VectorTools::interpolate_boundary_values(dof_handler,
                                             boundary_conditions.vorticity(),
                                             constraints,
                                             fe_handler.component_mask(VariableName::Vorticity));

    constraints.close();


}

template<int dim>
void NSESolver<dim>::run(){

}

template class NSESolver<2>;