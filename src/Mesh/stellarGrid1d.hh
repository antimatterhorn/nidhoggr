// stellarGrid1D.hh
#pragma once
#include <vector>

namespace Mesh {
class StellarGrid1d {
private:
    
public:
    int nz;
    std::vector<double> m;   // Enclosed mass at zone centers
    std::vector<double> r;  // Radius at zone centers

    StellarGrid1d(int numZones, double totalMass);

    double dm(int i) const;
    void computeRadiusFromMass(const std::vector<double>& rho);
    int size() const { return nz;}
    const std::vector<double>& getMasses() const { return m; }
    const std::vector<double>& getRadii() const { return r; }
};
}