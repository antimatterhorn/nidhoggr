// hydrostaticEquilibrium1D.hh
#pragma once

#include "physics.hh"
#include "../Mesh/stellarGrid1D.hh"

class HydrostaticEquilibrium : public Physics<1> {
public:
    StellarGrid1D* grid;
    PhysicalConstants& constants;

    HydrostaticEquilibrium(StellarGrid1D* grid, NodeList* nodeList, PhysicalConstants& constants);

    void evaluate();
};
