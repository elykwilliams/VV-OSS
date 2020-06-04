//
// Created by kylew on 5/25/20.
//

#ifndef NSE_TYPES_H
#define NSE_TYPES_H
#include <deal.II/base/types.h>
#include <deal.II/base/tensor.h>
#include <deal.II/fe/fe_values_extractors.h>

enum class TestCase {
    round_cylinder,
    channel_flow,
    manufactured
};

namespace BoundaryType {
    enum : dealii::types::boundary_id {
        inflow,
        outflow,
        wall,
        bluff
    };
}

// Define Types for vorticity depending on dimension
template <int dim>
struct NSETypes{};

template <>
struct NSETypes<2>
{

    typedef dealii::Tensor<1,1> CurlType;
    typedef double  VorticityType;
    typedef dealii::Tensor<1,2> VorticityGradType;

    typedef dealii::FEValuesExtractors::Scalar VorticityExtractorType;
    typedef dealii::FEValuesExtractors::Scalar PressureExtractorType;
    typedef dealii::FEValuesExtractors::Vector VelocityExtractorType;
};


template <>
struct NSETypes<3>
{
    typedef dealii::Tensor<1,3> CurlType;
    typedef dealii::Tensor<1,3> VorticityType;
    typedef dealii::Tensor<2,3> VorticityGradType;

    typedef dealii::FEValuesExtractors::Vector VorticityExtractorType;
    typedef dealii::FEValuesExtractors::Scalar PressureExtractorType;
    typedef dealii::FEValuesExtractors::Vector VelocityExtractorType;
};

namespace VariableName {
    enum : int {
        Velocity,
        Pressure,
        Vorticity
    };
}


#endif //NSE_TYPES_H
