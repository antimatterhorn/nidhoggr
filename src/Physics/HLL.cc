#pragma once
#include "physics.hh"

template<int dim>
struct HLLFlux {
    double mass;
    Lin::Vector<dim> momentum;
    double energy;
};

template<int dim>
HLLFlux<dim>
computeHLLFlux(int iL, int iR, int axis,
               const Field<double>& rho,
               const Field<Lin::Vector<dim>>& v,
               const Field<double>& u,
               const Field<double>& p,
               const Field<double>& cs) {
    using Vector = Lin::Vector<dim>;

    // Left state
    double rhoL = rho.getValue(iL);
    Vector vL = v.getValue(iL);
    double uL = u.getValue(iL);
    double pL = p.getValue(iL);
    double cL = cs.getValue(iL);
    Vector momL = vL * rhoL;
    double eL = uL + 0.5 * vL.mag2();
    double EL = rhoL * eL;

    // Right state
    double rhoR = rho.getValue(iR);
    Vector vR = v.getValue(iR);
    double uR = u.getValue(iR);
    double pR = p.getValue(iR);
    double cR = cs.getValue(iR);
    Vector momR = vR * rhoR;
    double eR = uR + 0.5 * vR.mag2();
    double ER = rhoR * eR;

    // Normal velocities
    double vnL = vL[axis];
    double vnR = vR[axis];

    // Fluxes for left and right states
    double massFluxL = rhoL * vnL;
    Vector momFluxL = momL * vnL;
    momFluxL[axis] += pL;
    double energyFluxL = vnL * (EL + pL);

    double massFluxR = rhoR * vnR;
    Vector momFluxR = momR * vnR;
    momFluxR[axis] += pR;
    double energyFluxR = vnR * (ER + pR);

    // Estimate wave speeds
    double sL = std::min(vnL - cL, vnR - cR);
    double sR = std::max(vnL + cL, vnR + cR);

    // HLL flux
    HLLFlux<dim> result;

    if (sL >= 0.0) {
        result.mass = massFluxL;
        result.momentum = momFluxL;
        result.energy = energyFluxL;
    } else if (sR <= 0.0) {
        result.mass = massFluxR;
        result.momentum = momFluxR;
        result.energy = energyFluxR;
    } else {
        double sRSum = sR - sL;
        result.mass     = (sR * massFluxL - sL * massFluxR + sR * sL * (rhoR - rhoL)) / sRSum;
        result.momentum = (sR * momFluxL - sL * momFluxR + sR * sL * (momR - momL)) / sRSum;
        result.energy   = (sR * energyFluxL - sL * energyFluxR + sR * sL * (ER - EL)) / sRSum;
    }

    return result;
}

template<int dim>
HLLFlux<dim>
computeHLLCFlux(int iL, int iR, int axis,
               const Field<double>& rho,
               const Field<Lin::Vector<dim>>& v,
               const Field<double>& u,
               const Field<double>& p,
               const Field<double>& cs) {
    using Vector = Lin::Vector<dim>;

    // Left state
    double rhoL = rho.getValue(iL);
    Vector vL = v.getValue(iL);
    double uL = u.getValue(iL);
    double pL = p.getValue(iL);
    double cL = cs.getValue(iL);
    Vector momL = vL * rhoL;
    double eL = uL + 0.5 * vL.mag2();
    double EL = rhoL * eL;

    // Right state
    double rhoR = rho.getValue(iR);
    Vector vR = v.getValue(iR);
    double uR = u.getValue(iR);
    double pR = p.getValue(iR);
    double cR = cs.getValue(iR);
    Vector momR = vR * rhoR;
    double eR = uR + 0.5 * vR.mag2();
    double ER = rhoR * eR;

    // Normal velocities
    double vnL = vL[axis];
    double vnR = vR[axis];

    // Fluxes for left and right states
    double massFluxL = rhoL * vnL;
    Vector momFluxL = momL * vnL;
    momFluxL[axis] += pL;
    double energyFluxL = vnL * (EL + pL);

    double massFluxR = rhoR * vnR;
    Vector momFluxR = momR * vnR;
    momFluxR[axis] += pR;
    double energyFluxR = vnR * (ER + pR);

    // Estimate wave speeds
    double sL = std::min(vnL - cL, vnR - cR);
    double sR = std::max(vnL + cL, vnR + cR);

    // HLL flux
    HLLFlux<dim> result;

    if (sL >= 0.0) {
        result.mass = massFluxL;
        result.momentum = momFluxL;
        result.energy = energyFluxL;
    } else if (sR <= 0.0) {
        result.mass = massFluxR;
        result.momentum = momFluxR;
        result.energy = energyFluxR;
    } else {
        double sRSum = sR - sL;
        result.mass     = (sR * massFluxL - sL * massFluxR + sR * sL * (rhoR - rhoL)) / sRSum;
        result.momentum = (sR * momFluxL - sL * momFluxR + sR * sL * (momR - momL)) / sRSum;
        result.energy   = (sR * energyFluxL - sL * energyFluxR + sR * sL * (ER - EL)) / sRSum;
    }

    return result;
}