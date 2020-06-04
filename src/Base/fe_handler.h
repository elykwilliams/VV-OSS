//
// Created by kylew on 6/3/20.
//

#ifndef NSE_FE_HANDLER_H
#define NSE_FE_HANDLER_H


#include <deal.II/fe/fe.h>
#include <deal.II/fe/fe_q.h>
#include <deal.II/fe/component_mask.h>

#include "Base/settings.h"
#include "Base/types.h"

#include <variant>
#include <map>

using std::vector, std::map, std::variant, std::string;
using std::shared_ptr, std::make_shared;

using namespace dealii;

template <int dim>
class FeHandlerBase
{
    // Provides a simple interface to add more unknown fields to an FESystem.
    // This class contains the data structures required to create an FeSystem.
    // It also helps manages the block structre with extractors and
    // componentmasks.

public:
    FeHandlerBase() : n_components(0), n_blocks(0){}

    unsigned int size(){ return n_components;}

    vector<unsigned int> multiplicities() const{
        AssertThrow(mult_vec.size() > 0, ExcNotInitialized());

        return mult_vec;
    }

    vector<const FiniteElement<dim> *> fes() const{
        AssertThrow(fe_vec.size() > 0, ExcNotInitialized());

        vector<const FiniteElement<dim> *>retvec;
        for(unsigned int i = 0; i < fe_vec.size(); ++i)
            retvec.push_back(fe_vec[i].get());

        return retvec;
    }

    ComponentMask component_mask(const VariableName fe_name){
        // Generate Componenet mask for given block number

        vector<bool> mask;
        for(int comp : components_to_blocks)
            mask.push_back(comp == block_name[fe_name]);

        return ComponentMask(mask);
    };

    auto extractor(VariableName fe_name){
        return extractor_[fe_name];
    }

protected:
    void add_scalar_fe(shared_ptr<FiniteElement<dim>> fe, int fe_name){
        // Adds extractor before  n_components increments

        extractor_[fe_name] = FEValuesExtractors::Scalar(n_components);
        block_name[fe_name] = n_blocks;

        fe_vec.push_back(fe);
        mult_vec.push_back(1);
        n_components += 1;
        components_to_blocks.push_back(n_blocks);
        ++n_blocks;
    }

    void add_vector_fe(shared_ptr<FiniteElement<dim>> fe, VariableName fe_name)
    {
        extractor_[fe_name] = FEValuesExtractors::Vector(n_components);
        block_name[fe_name] = n_blocks;

        fe_vec.push_back(fe);
        mult_vec.push_back(dim);
        n_components += dim;
        for (unsigned int i = 0; i < dim; ++i)
            components_to_blocks.push_back(n_blocks);
        ++n_blocks;

    }

    map<VariableName, variant<FEValuesExtractors::Scalar, FEValuesExtractors::Vector>>
            extractor_;

    // Convert component number to block number
    vector<unsigned int> components_to_blocks;

private:
    vector<shared_ptr<FiniteElement<dim>>> fe_vec{};    // Array of Finite Elements
    vector<unsigned int> mult_vec{};                    // Dimensions of Finite Elements
    map<int, int> block_name;
    unsigned int n_components;
    unsigned int n_blocks;

};


template <int dim>
class NSEFeHandler : private FeHandlerBase<dim>, public NSETypes<dim>
{
    // This class ia a FeHandlerBase for flow problems with or without
    // vorticity. The reason for this class is to manage the cases when
    // vorticity 1) doesn't exist, 2) is a scalar, 3) is a vector.
    // Provides a simple interface to add more unknown fields.
public:

    NSEFeHandler(const NSEFeHandler<dim> &feh) = default;

    explicit NSEFeHandler(const FEParameters& s)
        : NSEFeHandler(s.vel_deg, s.pres_deg, s.have_vort){}

    NSEFeHandler(int velocity_degree, int pressure_degree, bool have_vorticity) :
        is_stable(velocity_degree == pressure_degree + 1)
    {
        AssertThrow(velocity_degree > 0 && pressure_degree > 0, ExcNotInitialized());

        // Add Velocity
        this->add_vector_fe(make_shared<FE_Q<dim>>(velocity_degree), NSEFeHandler<dim>::Velocity);

        // Add Pressure
        this->add_scalar_fe(make_shared<FE_Q<dim>>(pressure_degree), NSEFeHandler<dim>::Pressure);

        // Add Vorticity
        if (dim == 3 && have_vorticity)
            this->add_vector_fe(make_shared<FE_Q<dim>>(velocity_degree), NSEFeHandler<dim>::Vorticity);

        else if (dim == 2 && have_vorticity)
            this->add_scalar_fe(make_shared<FE_Q<dim>>(velocity_degree), NSEFeHandler<dim>::Vorticity);
    }

    const bool is_stable;

};


#endif //NSE_FE_HANDLER_H
