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
    exact_solution(s.testcaseSettings, fe_handler)
{
    struct stat sb{};
    if (stat(generalSettings.output_dir.c_str(), &sb) != 0)
        mkdir(generalSettings.output_dir.c_str(), 0777);

    setup_mesh();
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
        pcout << "mesh was saved" << std::endl;
    }
}

template<int dim>
void NSESolver<dim>::setup_system(){



}


template class NSESolver<2>;