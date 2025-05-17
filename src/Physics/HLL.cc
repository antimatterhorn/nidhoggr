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
computeHLLEFlux(int iL, int iR, int axis,
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

//LEGACY UNLIMITED VERSION
template<int dim>
HLLFlux<dim>
computeHLLCFlux(int iL, int iR, int axis,
                const Field<double>& rho,
                const Field<Lin::Vector<dim>>& v,
                const Field<double>& u,
                const Field<double>& p,
                const Field<double>& cs) {
    using Vector = Lin::Vector<dim>;

    constexpr double tiny = 1e-12;
    constexpr double rho_floor = 1e-12;

    // Left state
    double rhoL = std::max(rho.getValue(iL), rho_floor);
    Vector vL   = v.getValue(iL);
    double uL   = std::max(u.getValue(iL), rho_floor);
    double pL   = p.getValue(iL);
    double cL   = cs.getValue(iL);
    Vector momL = rhoL * vL;
    double eL   = uL + 0.5 * vL.mag2();
    double EL   = rhoL * eL;
    double vnL  = vL[axis];

    // Right state
    double rhoR = std::max(rho.getValue(iR), rho_floor);
    Vector vR   = v.getValue(iR);
    double uR   = std::max(u.getValue(iR), rho_floor);
    double pR   = p.getValue(iR);
    double cR   = cs.getValue(iR);
    Vector momR = rhoR * vR;
    double eR   = uR + 0.5 * vR.mag2();
    double ER   = rhoR * eR;
    double vnR  = vR[axis];

    // Estimate wave speeds
    double sL = std::min(vnL - cL, vnR - cR);
    double sR = std::max(vnL + cL, vnR + cR);

    // Estimate contact wave speed
    double numerator   = pR - pL + rhoL * vnL * (sL - vnL) - rhoR * vnR * (sR - vnR);
    double denominator = rhoL * (sL - vnL) - rhoR * (sR - vnR);
    if (std::abs(denominator) < tiny)
        denominator = (denominator >= 0.0) ? tiny : -tiny;
    double sStar = numerator / denominator;

    // Compute fluxes
    double massFluxL = rhoL * vnL;
    Vector momFluxL = vnL * momL;
    momFluxL[axis] += pL;
    double energyFluxL = vnL * (EL + pL);

    double massFluxR = rhoR * vnR;
    Vector momFluxR = vnR * momR;
    momFluxR[axis] += pR;
    double energyFluxR = vnR * (ER + pR);

    HLLFlux<dim> flux;

    if (sL >= 0.0) {
        flux.mass     = massFluxL;
        flux.momentum = momFluxL;
        flux.energy   = energyFluxL;
        return flux;
    }

    if (sR <= 0.0) {
        flux.mass     = massFluxR;
        flux.momentum = momFluxR;
        flux.energy   = energyFluxR;
        return flux;
    }

    // Star region calculations
    double denomL = std::max(sL - sStar, tiny);
    double denomR = std::max(sR - sStar, tiny);

    double rhoSL = std::max(rhoL * (sL - vnL) / denomL, rho_floor);
    double rhoSR = std::max(rhoR * (sR - vnR) / denomR, rho_floor);

    Vector n = unitAxis<dim>(axis);
    Vector vL_star = vL - n * vnL;
    Vector vR_star = vR - n * vnR;

    Vector momSL = momL + (sStar - vnL) * (rhoSL * vL_star);
    Vector momSR = momR + (sStar - vnR) * (rhoSR * vR_star);

    double ESL = EL + (sStar - vnL) * (rhoSL * (eL + 0.5 * (sStar - vnL)));
    double ESR = ER + (sStar - vnR) * (rhoSR * (eR + 0.5 * (sStar - vnR)));

    if (sStar >= 0.0) {
        flux.mass     = massFluxL + sL * (rhoSL - rhoL);
        flux.momentum = momFluxL + sL * (momSL - momL);
        flux.energy   = energyFluxL + sL * (ESL - EL);
    } else {
        flux.mass     = massFluxR + sR * (rhoSR - rhoR);
        flux.momentum = momFluxR + sR * (momSR - momR);
        flux.energy   = energyFluxR + sR * (ESR - ER);
    }

    // Final check: if any flux component is NaN, print warning
    if (!std::isfinite(flux.mass) ||
        !std::isfinite(flux.energy) ||
        !std::isfinite(flux.momentum[0])) {
        std::cerr << "HLLC NaN: sStar=" << sStar
                  << ", rhoL=" << rhoL << ", rhoR=" << rhoR
                  << ", vnL=" << vnL << ", vnR=" << vnR
                  << ", sL=" << sL << ", sR=" << sR
                  << ", denom=" << denominator << std::endl;
    }

    return flux;
}

//LIMITED VERSION
template<int dim>
HLLFlux<dim> computeHLLCFluxFromStates(
    double rhoL, const Lin::Vector<dim>& vL, double uL, double pL, double cL,
    double rhoR, const Lin::Vector<dim>& vR, double uR, double pR, double cR,
    int axis) {

    using Vector = Lin::Vector<dim>;

    Vector momL = rhoL * vL;
    Vector momR = rhoR * vR;
    double eL = uL + 0.5 * vL.mag2();
    double eR = uR + 0.5 * vR.mag2();
    double EL = rhoL * eL;
    double ER = rhoR * eR;
    double vnL = vL[axis];
    double vnR = vR[axis];

    // Estimate wave speeds
    double sL = std::min(vnL - cL, vnR - cR);
    double sR = std::max(vnL + cL, vnR + cR);

    // Contact wave speed estimate (Toro)
    double numerator = pR - pL + rhoL * vnL * (sL - vnL) - rhoR * vnR * (sR - vnR);
    double denominator = rhoL * (sL - vnL) - rhoR * (sR - vnR);
    const double tiny = 1e-12;
    if (std::abs(denominator) < tiny)
        denominator = (denominator >= 0.0) ? tiny : -tiny;
    double sStar = numerator / denominator;

    // Left flux
    double massFluxL = rhoL * vnL;
    Vector momFluxL = vnL * momL; momFluxL[axis] += pL;
    double energyFluxL = vnL * (EL + pL);

    // Right flux
    double massFluxR = rhoR * vnR;
    Vector momFluxR = vnR * momR; momFluxR[axis] += pR;
    double energyFluxR = vnR * (ER + pR);

    HLLFlux<dim> flux;
    if (sL >= 0.0) {
        flux.mass = massFluxL;
        flux.momentum = momFluxL;
        flux.energy = energyFluxL;
    } else if (sR <= 0.0) {
        flux.mass = massFluxR;
        flux.momentum = momFluxR;
        flux.energy = energyFluxR;
    } else {
        Vector n = unitAxis<dim>(axis);

        double denomL = std::max(sL - sStar, tiny);
        double denomR = std::max(sR - sStar, tiny);

        double rhoSL = std::max(rhoL * (sL - vnL) / denomL, tiny);
        double rhoSR = std::max(rhoR * (sR - vnR) / denomR, tiny);

        Vector vL_star = vL - n * vnL;
        Vector vR_star = vR - n * vnR;

        Vector momSL = momL + (sStar - vnL) * (rhoSL * vL_star);
        Vector momSR = momR + (sStar - vnR) * (rhoSR * vR_star);

        double ESL = EL + (sStar - vnL) * (rhoSL * (eL + 0.5 * (sStar - vnL)));
        double ESR = ER + (sStar - vnR) * (rhoSR * (eR + 0.5 * (sStar - vnR)));

        if (sStar >= 0.0) {
            flux.mass = massFluxL + sL * (rhoSL - rhoL);
            flux.momentum = momFluxL + sL * (momSL - momL);
            flux.energy = energyFluxL + sL * (ESL - EL);
        } else {
            flux.mass = massFluxR + sR * (rhoSR - rhoR);
            flux.momentum = momFluxR + sR * (momSR - momR);
            flux.energy = energyFluxR + sR * (ESR - ER);
        }
    }

    return flux;
}
