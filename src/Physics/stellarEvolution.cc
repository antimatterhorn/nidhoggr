// hydrostaticEquilibrium1D.cc
#include "stellarEvolution.hh"

StellarEvolution::StellarEvolution(StellarGrid1d* grid, NodeList* nodeList, PhysicalConstants& constants)
    : Physics<1>(nodeList, constants), grid(grid), constants(constants) {
    if (nodeList->getField<double>("pressure") == nullptr)
        nodeList->insertField<double>("pressure");
    if (nodeList->getField<double>("density") == nullptr)
        nodeList->insertField<double>("density");
}

void StellarEvolution::evaluate() {
    Field<double>* P = nodeList->getField<double>("pressure");
    Field<double>* rho = nodeList->getField<double>("density");

    const int nz = grid->nz;
    P->setValue(nz - 1, 0.0);  // Surface boundary condition

    for (int i = nz - 2; i >= 0; --i) {
        double r_next = grid->r[i + 1];
        double dPdm = -constants.G() * grid->m[i + 1] / (4 * M_PI * std::pow(r_next, 4));
        double Pi = (*P)[i + 1] - dPdm * grid->dm(i);
        P->setValue(i, Pi);
    }
}
