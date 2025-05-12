// stellarGrid1D.hh
#pragma once
#include <vector>

class StellarGrid1d {
public:
    int nz;
    std::vector<double> m;  // Enclosed mass at zone centers
    std::vector<double> r;  // Radius at zone centers

    StellarGrid1d(int numZones, double totalMass);

    double dm(int i) const;
    void computeRadiusFromMass(const std::vector<double>& rho);
};
