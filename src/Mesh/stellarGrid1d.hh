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

    inline double dm(int i) const {
        if (i == 0) return m[1] - m[0];
        return m[i] - m[i - 1];
    }
    void computeRadiusFromMass(const std::vector<double>& rho);
    int size() const { return nz;}
    const std::vector<double>& getMasses() const { return m; }
    const std::vector<double>& getRadii() const { return r; }
};
}