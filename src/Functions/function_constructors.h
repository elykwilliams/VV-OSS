//
// Created by kylew on 6/8/20.
//

#ifndef NSE_RIGHHANDSIDE_H
#define NSE_RIGHHANDSIDE_H

#include "Base/settings.h"
#include "Base/fe_handler.h"

#include "Functions/manufactured.h"

#include <deal.II/base/tensor_function.h>


using namespace dealii;

template<int dim>
class ForcingFunction{
public:
    ForcingFunction(const TestCaseParameters& s, const FeHandlerBase<dim>& feh){
        switch(s.name){
            case TestCase::manufactured:
                velocity_function.reset(new ManufacturedForcing<dim, double>(s.nu, feh.size()));
                vorticity_function.reset(new ManufacturedForcingCurl<dim>(s.nu));
                break;
            case TestCase::channel_flow:

                break;
            case TestCase::round_cylinder:

                break;
        }

    };

    std::shared_ptr<TensorFunction<1, dim, double>> velocity(){
        return velocity_function;
    }

    std::shared_ptr<Function<dim>> vorticity(){
        return vorticity_function;
    }

private:
    std::shared_ptr<TensorFunction<1, dim, double>> velocity_function{};
    std::shared_ptr<Function<dim>> vorticity_function{};
};

template<int dim>
class AnalyticSolution{
public:
    AnalyticSolution(const TestCaseParameters& s, const FeHandlerBase<dim>& feh){
        switch(s.name){
            case TestCase::manufactured:
                solution.reset(new ManufacturedSolution<dim>(s, feh));
                break;
            case TestCase::channel_flow:

                break;
            case TestCase::round_cylinder:

                break;
        }
    }

    double value(const Point<dim> &p, const unsigned int comp) const{
        return solution->value(p, comp);
    }

private:
    std::shared_ptr<Function<dim>> solution{};

};


template<int dim>
class BoundaryConditions{
public:
    BoundaryConditions(const TestCaseParameters& s, const FeHandlerBase<dim>& feh){
        switch(s.name){
            case TestCase::manufactured:
                velocity_dirichlet[BoundaryType::dirichlet] = new ManufacturedSolution<2>(s, feh);
                vorticity_dirichlet[BoundaryType::dirichlet] = new ManufacturedSolution<2>(s, feh);
                break;
            case TestCase::channel_flow:

                break;
            case TestCase::round_cylinder:

                break;
        }

    };

    ~BoundaryConditions(){
        for(auto elem : velocity_dirichlet){
            delete elem.second;
        }

        for(auto elem : vorticity_dirichlet){
            delete elem.second;
        }
    }

    auto velocity(){
        return velocity_dirichlet;
    }
    auto vorticity(){
        return velocity_dirichlet;
    }

    void update_vorticity(){ ;}

private:
    std::map<types::boundary_id, const dealii::Function<dim>*> velocity_dirichlet;
    std::map<types::boundary_id, const dealii::Function<dim>*> vorticity_dirichlet;
};


#endif //NSE_RIGHHANDSIDE_H
