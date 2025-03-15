
#ifndef PHYSICALCONSTANTS_HH
#define PHYSICALCONSTANTS_HH

#include <complex>
#include <iostream>

class PhysicalConstants {
private:


    // The reference MKS data we base our values on.
    const double mpMKS =     1.67262192369e-27;         // kg
    const double meMKS =     9.1093837015e-31;          // kg
    const double qeMKS =     1.602176634e-19;           // Coulombs
    const double GMKS =      6.67430e-11;               // N*m^2/kg^2
    const double cMKS =      2.99792458e8;              // m/s
    const double kBMKS =     1.380649e-23;              // J/K
    const double RgasMKS =   8.314462618;               // J/mole/K
    const double NAvogadro = 6.02214076e23;             // mol^-1
    const double StefanBoltzmannMKS = 5.670374419e-8;   // W/m^2/K^4
    const double PlanckMKS = 6.62607015e-34;            // J*s
    const double EpsilonZeroMKS = 8.85418782e-12;       // C^2*s^2/kg/m^3
    const double EGravMKS =  9.81;                      // m/s/s

    // Dependent variables.
    double UnitEnergyJ;
    double ProtonMass;
    double ElectronMass;
    double ElectronCharge;
    double GGravity;
    double cLight;
    double kBoltzmann;
    double MolarGasConstant;
    double KelvinsToEnergyPerMole;
    double UnitMassDensity;
    double Sigma;
    double BlackBody;
    double Planck;
    double EpsilonZero;

public:
    // Independent variables.
    double mUnitLm, mUnitMkg, mUnitTsec, mUnitTeK, mUnitCcou;
    
    PhysicalConstants(const double unitLm,
                        const double unitMkg,
                        const double unitTsec,
                        const double unitTeK,
                        const double unitCcou)
        : mUnitLm(unitLm),
        mUnitMkg(unitMkg),
        mUnitTsec(unitTsec),
        mUnitTeK(unitTeK),
        mUnitCcou(unitCcou) {
            std::cout << "constants constructed" << std::endl;
            GGravity = GMKS / (mUnitLm / mUnitMkg * pow(mUnitLm / mUnitTsec, 2.0));
    }

    // The fundamental independent quantities.
    double unitLengthMeters() const { return mUnitLm; }
    double unitMassKg() const { return mUnitMkg; }
    double unitTimeSec() const { return mUnitTsec; }
    double unitTemperatureKelvin() const { return mUnitTeK; }
    double unitChargeCoulomb() const { return mUnitCcou; }

    // All the stuff we provide.
    double protonMass() const { return mpMKS / mUnitMkg; }
    double electronMass() const { return meMKS / mUnitMkg; }
    double electronCharge() const { return qeMKS / mUnitCcou; }
    double G() const { return GGravity; }
    double c() const { return cMKS / (mUnitLm / mUnitTsec); }
    double kB() const { return kBMKS * mUnitTeK / (mUnitMkg * pow(mUnitLm / mUnitTsec, 2.0)); }
    double molarGasConstant() const { return RgasMKS * mUnitTeK / (mUnitMkg * pow(mUnitLm / mUnitTsec, 2.0)); }
    double kelvinsToEnergyPerMole() const { return mUnitMkg * pow(mUnitLm / mUnitTsec, 2.0) / (kBMKS * mUnitTeK) * NAvogadro; }
    double unitMassDensity() const { return mUnitMkg / (mUnitLm * mUnitLm * mUnitLm); }
    double stefanBoltzmannConstant() const { return StefanBoltzmannMKS * mUnitTeK * mUnitTeK * mUnitTeK * mUnitTeK / (mUnitMkg * mUnitTsec * mUnitTsec * mUnitTsec); }
    double blackBodyConstant() const { return 4 * StefanBoltzmannMKS * mUnitTeK * mUnitTeK * mUnitTeK * mUnitTeK / (cMKS * mUnitTsec * mUnitTsec * mUnitLm / mUnitMkg); }
    double planckConstant() const { return PlanckMKS * mUnitTsec / (mUnitMkg * mUnitLm * mUnitLm); }
    double unitEnergyJ() const { return mUnitMkg * pow(mUnitLm / mUnitTsec, 2.0); }
    double epsilonZero() const { return EpsilonZeroMKS * pow(mUnitLm, 3.0) * mUnitMkg / (pow(mUnitTsec, 2.0) * pow(mUnitCcou, 2.0)); }
    double ESurfaceGrav() const { return EGravMKS / (mUnitLm / pow(mUnitTsec,2.0)); }

    // Method to set G directly
    void setG(double newG) { GGravity = newG; }
};

#endif // PHYSICALCONSTANTS_HH