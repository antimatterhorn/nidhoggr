// stellarEvolution.hh
#pragma once

#include "physics.hh"
#include "../Mesh/stellarGrid1d.hh"

class StellarEvolution : public Physics<1> {
public:
    StellarGrid1d* grid;
    PhysicalConstants& constants;

    StellarEvolution(StellarGrid1d* grid, NodeList* nodeList, PhysicalConstants& constants);

    void evaluate();
};
