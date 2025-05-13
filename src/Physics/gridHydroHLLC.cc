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

    virtual
    HLLFlux<dim> computeFlux(int iL, int iR, int axis,
        const Field<double>& rho, const Field<Vector>& v,
        const Field<double>& u, const Field<double>& p,
        const Field<double>& cs) const override {

        using Vector = Lin::Vector<dim>;

        auto neighborsL = this->grid->getNeighboringCells(iL);
        auto neighborsR = this->grid->getNeighboringCells(iR);

        int iLL = neighborsL[2 * axis];  // cell before iL in axis
        int iRR = neighborsR[2 * axis + 1];  // cell after iR in axis

        auto minmod = [](double a, double b) {
            return (a * b <= 0.0) ? 0.0 : ((std::abs(a) < std::abs(b)) ? a : b);
        };

        auto minmodVec = [&](Vector a, Vector b) {
            Vector result;
            for (int d = 0; d < dim; ++d)
                result[d] = minmod(a[d], b[d]);
            return result;
        };

        // Center values
        double rhoL0 = rho.getValue(iL);
        double rhoR0 = rho.getValue(iR);
        double uL0 = u.getValue(iL);
        double uR0 = u.getValue(iR);
        Vector vL0 = v.getValue(iL);
        Vector vR0 = v.getValue(iR);

        // Slopes
        double srhoL = minmod(rho.getValue(iL) - rho.getValue(iLL), rho.getValue(iR) - rho.getValue(iL));
        double srhoR = minmod(rho.getValue(iR) - rho.getValue(iL), rho.getValue(iRR) - rho.getValue(iR));

        double suL = minmod(u.getValue(iL) - u.getValue(iLL), u.getValue(iR) - u.getValue(iL));
        double suR = minmod(u.getValue(iR) - u.getValue(iL), u.getValue(iRR) - u.getValue(iR));

        Vector svL = minmodVec(v.getValue(iL) - v.getValue(iLL), v.getValue(iR) - v.getValue(iL));
        Vector svR = minmodVec(v.getValue(iR) - v.getValue(iL), v.getValue(iRR) - v.getValue(iR));

        // Reconstruct states at the interface
        double rhoL = rhoL0 + 0.5 * srhoL;
        double rhoR = rhoR0 - 0.5 * srhoR;
        double uL   = uL0   + 0.5 * suL;
        double uR   = uR0   - 0.5 * suR;
        Vector vL   = vL0   + 0.5 * svL;
        Vector vR   = vR0   - 0.5 * svR;

        rhoL = std::max(rhoL, 1e-12);
        rhoR = std::max(rhoR, 1e-12);
        uL = std::max(uL, 1e-12);
        uR = std::max(uR, 1e-12);

        double pL = p.getValue(iL);
        double pR = p.getValue(iR);
        double cL = cs.getValue(iL);
        double cR = cs.getValue(iR);

        return computeHLLCFluxFromStates<dim>(rhoL, vL, uL, pL, cL,
                                            rhoR, vR, uR, pR, cR, axis);
    }
};
