//
// Created by kylew on 3/12/20.
//

#ifndef NSE_SETTINGS_H
#define NSE_SETTINGS_H
#include <deal.II/base/parameter_handler.h>
#include <deal.II/base/parameter_acceptor.h>

#include <string>
using std::string;
#include <fstream>
using std::ofstream;

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


struct Settings
{
    explicit Settings(const string& filename): gp(prm), tcp(prm){

        prm.parse_input(filename);

        ofstream ofs{filename};
        prm.print_parameters(ofs, ParameterHandler::OutputStyle::Text);

    }


private:
    ParameterHandler prm;
public:
    GeneralParameters gp;
    TestCaseParameters tcp;
};

#endif //NSE_SETTINGS_H
