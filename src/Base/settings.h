//
// Created by kylew on 3/12/20.
//

#ifndef NSE_SETTINGS_H
#define NSE_SETTINGS_H
#include <deal.II/base/parameter_handler.h>
#include <deal.II/base/mpi.h>
#include <deal.II/base/conditional_ostream.h>

#include "Base/types.h"

#include <string>
#include <fstream>
#include <sys/stat.h>

using std::ofstream, std::string;

using namespace dealii;

struct TestCaseParameters
{

    explicit TestCaseParameters(ParameterHandler& prm) {
        prm.enter_subsection("Test Case Parameters");
        prm.add_parameter("Test Case Type", name_str, "round_cylinder | channel_flow | manufactured",
                Patterns::Selection("round_cylinder|channel_flow|manufactured"));
        prm.add_action("Test Case Type", [&](const string& s){
           if (s == "round_cylinder") name = TestCase::round_cylinder;
           else if (s == "channel_flow") name = TestCase::channel_flow;
           else if (s == "manufactured") name = TestCase::manufactured;
        });
        prm.leave_subsection();
    }

    string name_str = "manufactured";
    TestCase name;
};


struct GeneralParameters
{
    explicit GeneralParameters(ParameterHandler& prm) {
        prm.enter_subsection("General Settings");
        prm.add_parameter("Output Directory", output_dir);
        prm.add_action("Output Directory", [&](string s){
          output_dir = output_dir + (s.back() != '/' ? "/" : "" );
        });
        prm.add_parameter("Dimension", dim);
        prm.leave_subsection();

        mkdir(output_dir.c_str(), 0777);
    }

    string output_dir = "./output/";
    int dim = 2;
};

struct MeshParameters
{
    explicit MeshParameters(ParameterHandler& prm) {
        prm.enter_subsection("Mesh Settings");
        prm.add_parameter("Output Mesh", output_mesh);
        prm.add_parameter("Initial Refinements", initial_refinements);
        prm.leave_subsection();
    }

    bool output_mesh = true;
    int initial_refinements = 3;
};

struct FEParameters
{
    explicit FEParameters(ParameterHandler& prm){
        prm.enter_subsection("FE Settings");
        prm.add_parameter("Velocity Degree", vel_deg);
        prm.add_parameter("Pressure Degree", pres_deg);
        prm.add_parameter("Use vorticity", have_vort);
        prm.leave_subsection();
        is_lbb_stable = (vel_deg == pres_deg + 1);
    }

    unsigned int vel_deg = 2;
    unsigned int pres_deg = 1;
    bool have_vort = false;
    bool is_lbb_stable = true;
};
struct Settings
{
    explicit Settings(const string& filename):
            parameter_file(filename),
            generalSettings(prm),
            testcaseSettings(prm),
            meshSettings(prm),
            feSettings(prm)
    {
        prm.parse_input(filename);

        std::size_t pos = filename.find(".prm");
        filename_base = filename.substr(0, pos);
    }

    Settings(const Settings& s):
            prm(),
            parameter_file(s.parameter_file),
            generalSettings(s.generalSettings),
            testcaseSettings(s.testcaseSettings),
            meshSettings(s.meshSettings),
            feSettings(s.feSettings)
    {
        std::size_t pos = parameter_file.find(".prm");
        filename_base = parameter_file.substr(0, pos);
    }

    int get_dim(){
        return generalSettings.dim;
    }

    void export_settings(const string& filename){
        ofstream ofs{filename};
        prm.print_parameters(ofs, ParameterHandler::OutputStyle::Text);
    }

private:
    ParameterHandler prm;
public:
    string parameter_file;
    string filename_base;
    GeneralParameters generalSettings;
    TestCaseParameters testcaseSettings;
    MeshParameters meshSettings;
    FEParameters feSettings;
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
