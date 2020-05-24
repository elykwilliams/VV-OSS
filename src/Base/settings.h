//
// Created by kylew on 3/12/20.
//

#ifndef NSE_SETTINGS_H
#define NSE_SETTINGS_H
#include <deal.II/base/parameter_handler.h>
#include <deal.II/base/parameter_acceptor.h>

#include <string>
#include <fstream>
using std::ofstream, std::string;

using namespace dealii;

struct TestCaseParameters
{
    explicit TestCaseParameters(ParameterHandler& prm) {
        prm.enter_subsection("Test Case Parameters");
        prm.leave_subsection();
    }
};


struct GeneralParameters
{
    explicit GeneralParameters(ParameterHandler& prm) {
        prm.enter_subsection("General Settings");
        prm.add_parameter("Output Directory", output_dir);
        prm.add_parameter("Dimension", dim);
        prm.leave_subsection();
    }

    string output_dir = "";
    int dim = 3;
};

struct MeshParameters
{
    explicit MeshParameters(ParameterHandler& prm) {
        prm.enter_subsection("Mesh Settings");
        prm.add_parameter("Output Mesh", output_mesh);
        prm.add_parameter("Initial Refinements", initial_refinements);
        prm.leave_subsection();
    }

    bool output_mesh = false;
    int initial_refinements = 3;
};


struct Settings
{
    explicit Settings(const string& filename):
        GeneralSettings(prm),
        TestCaseSettings(prm),
        MeshSettings(prm)
    {
        prm.parse_input(filename);

        ofstream ofs{filename};
        prm.print_parameters(ofs, ParameterHandler::OutputStyle::Text);
    }

    Settings(const Settings& s):
        prm(),
        GeneralSettings(s.GeneralSettings),
        TestCaseSettings(s.TestCaseSettings),
        MeshSettings(s.MeshSettings)
    {}

    int get_dim(){
        return GeneralSettings.dim;
    }

private:
    ParameterHandler prm;
public:
    GeneralParameters GeneralSettings;
    TestCaseParameters TestCaseSettings;
    MeshParameters MeshSettings;
};


#include <deal.II/base/mpi.h>
#include <deal.II/base/conditional_ostream.h>
struct MPISettings{
    MPISettings():
        mpi_comm(MPI_COMM_WORLD),
        rank(static_cast<int>(Utilities::MPI::this_mpi_process(mpi_comm))),
        pcout(std::cout, rank == 0)
    {

    }

    MPI_Comm mpi_comm;
    int rank;
    ConditionalOStream pcout;

};

#endif //NSE_SETTINGS_H
