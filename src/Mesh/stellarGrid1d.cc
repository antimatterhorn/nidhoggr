// stellarGrid1D.cc
#include "stellarGrid1d.hh"
#include <cmath>
namespace Mesh {
StellarGrid1d::StellarGrid1d(int numZones, double totalMass) : nz(numZones) {
    m.resize(nz);
    r.resize(nz);
    double dM = totalMass / nz;
    for (int i = 0; i < nz; ++i)
        m[i] = (i + 0.5) * dM;
}

void StellarGrid1d::computeRadiusFromMass(const std::vector<double>& rho) {
    r[0] = 1e-5;  // Small seed radius
    for (int i = 1; i < nz; ++i) {
        double drdm = 1.0 / (4.0 * M_PI * r[i - 1] * r[i - 1] * rho[i - 1]);
        r[i] = r[i - 1] + drdm * dm(i);
    }
}
}