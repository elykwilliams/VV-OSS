//
// Created by kylew on 9/1/20.
//

#ifndef NSE_MANUFACTURED_H
#define NSE_MANUFACTURED_H

#include <deal.II/base/tensor_function.h>

using namespace dealii;

const double pi = numbers::PI;

/*******************************
 * Manufactured RHS
 ******************************/

template<int dim, class T>
class ManufacturedForcing : public TensorFunction<1, dim, T> {
public:

    ManufacturedForcing(int n_components, double nu)
            : TensorFunction<1, dim, T>(n_components), nu(nu) {}

    typename TensorFunction<1, dim, T>::value_type value(const Point<dim> &/*p*/) const override {
        return Tensor<1, dim>();
    }

private:
    double nu;
};

template<> inline
TensorFunction<1, 2, double>::value_type ManufacturedForcing<2, double>::value(const Point<2> &p) const {
    TensorFunction<1, 2, double>::value_type ret{};

    ret[0] = 4 * nu * pi * pi * sin(2 * pi * p[1])
             + 2 * pi * cos(2 * pi * p[0]) * cos(2 * pi * p[1])
             + 2 * pi * cos(2 * pi * p[0]);
    ret[1] = 4 * nu * pi * pi * cos(2 * pi * p[0])
             - 2 * pi * sin(2 * pi * p[1]) * sin(2 * pi * p[0])
             - 2 * pi * sin(2 * pi * p[1]);

    return ret;
}



template<int dim>
class ManufacturedForcingCurl : public Function<dim> {
public:
    explicit ManufacturedForcingCurl(double nu) : Function<dim>(1), nu(nu) {}

    double value(const Point<dim> &/*p*/,
                 const unsigned int /*component*/) const override { return 0; }

private:
    double nu;
};


template<> inline
double ManufacturedForcingCurl<2>::value(const Point<2> &p,
                                         const unsigned int component) const {
    Assert (component < this->n_components, ExcIndexRange(component, 0, this->n_components));
    return -nu * 8 * pi * pi * pi * (sin(2 * pi * p[0]) + cos(2 * pi * p[1]));
}


/*******************************
 * Analytic Solution
 ******************************/
template <int dim>
class ManufacturedSolution : public Function<dim>{
public:
    ManufacturedSolution(const TestCaseParameters& s, const FeHandlerBase<dim>& feh)
    : Function<dim>(feh.size()), nu(s.nu){ }

    double value(const Point<2> &, const unsigned int ) const override{
        return 0;
    }
private:
    const double nu;
};


template<> inline
double ManufacturedSolution<2>::value(const Point<2> &p,
                                      const unsigned int component) const {
    Assert (component < this->n_components,
            ExcIndexRange(component, 0, this->n_components));
    double x = p[0], y = p[1];

    switch (component) {
        case 0:
            return sin(2 * pi * y);
        case 1:
            return cos(2 * pi * x);
        case 2:
            return sin(2 * pi * x) + cos(2 * pi * y);
        case 3:
            return -2 * pi * sin(2 * pi * x) - 2 * pi * cos(2 * pi * y);
        default:
            break;
    }

    return 0;

}

#endif //NSE_MANUFACTURED_H
