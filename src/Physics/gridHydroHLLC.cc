#pragma once
#include "gridHydroBase.hh"
#include "HLL.cc"

template<int dim>
class GridHydroHLLC : public GridHydroBase<dim> {
public:
    using typename GridHydroBase<dim>::Vector;
    using typename GridHydroBase<dim>::VectorField;
    using typename GridHydroBase<dim>::ScalarField;

    GridHydroHLLC(NodeList* nodeList, PhysicalConstants& constants,
                  EquationOfState* eos, Mesh::Grid<dim>* grid)
        : GridHydroBase<dim>(nodeList, constants, eos, grid) {}

    virtual std::string name() const override { return "GridHydroHLLC"; }

    virtual HLLFlux<dim> computeFlux(int iL, int iR, int axis,
                                     const Field<double>& rho,
                                     const Field<Vector>& v,
                                     const Field<double>& u,
                                     const Field<double>& p,
                                     const Field<double>& cs) const override {
        return computeHLLCFlux<dim>(iL, iR, axis, rho, v, u, p, cs);
    }
};
