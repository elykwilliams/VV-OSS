//
// Created by kylew on 3/12/20.
//

#ifndef NSE_SETTINGS_H
#define NSE_SETTINGS_H
#include <deal.II/base/parameter_handler.h>
#include <deal.II/base/mpi.h>
#include <deal.II/base/conditional_ostream.h>

#include <string>
#include <fstream>
using std::ofstream, std::string;

using namespace dealii;

struct TestCaseParameters
{
    enum class type {round_cylinder, channel_flow, manufactured};
    explicit TestCaseParameters(ParameterHandler& prm) {
        prm.enter_subsection("Test Case Parameters");
        prm.add_parameter("Test Case Type", name_str, "round_cylinder | channel_flow | manufactured",
                Patterns::Selection("round_cylinder|channel_flow|manufactured"));
        prm.add_action("Test Case Type", [&](const string& s){
           if (s == "round_cylinder") name = type::round_cylinder;
           else if (s == "channel_flow") name = type::channel_flow;
           else if (s == "manufactured") name = type::manufactured;
        });
        prm.leave_subsection();
    }

    string name_str = "manufactured";
    type name;
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

    void export_settings(const string& filename){
        ofstream ofs{filename};
        prm.print_parameters(ofs, ParameterHandler::OutputStyle::Text);
    }

private:
    ParameterHandler prm;
public:
    GeneralParameters GeneralSettings;
    TestCaseParameters TestCaseSettings;
    MeshParameters MeshSettings;
};


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
