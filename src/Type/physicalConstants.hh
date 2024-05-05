
#ifndef PHYSICALCONSTANTS_HH
#define PHYSICALCONSTANTS_HH

#include <complex>
#include <iostream>

class PhysicalConstants {
private:
    // Independent variables.
    double mUnitLm, mUnitMkg, mUnitTsec, mUnitTeK, mUnitCcou;

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

    // Dependent variables.
    const double UnitEnergyJ;
    const double ProtonMass;
    const double ElectronMass;
    const double ElectronCharge;
    const double GGravity;
    const double cLight;
    const double kBoltzmann;
    const double MolarGasConstant;
    const double KelvinsToEnergyPerMole;
    const double UnitMassDensity;
    const double Sigma;
    const double BlackBody;
    const double Planck;
    const double EpsilonZero;

public:
    PhysicalConstants(const double unitLm,
                    const double unitMkg,
                    const double unitTsec,
                    const double unitTeK = 1.0,
                    const double unitCcou = 1.0): // Coulomb > Ampere
        mUnitLm(unitLm),
        mUnitMkg(unitMkg),
        mUnitTsec(unitTsec),
        mUnitTeK(unitTeK),
        mUnitCcou(unitCcou),
        UnitEnergyJ(unitMkg * pow(unitLm / unitTsec,2.0)),
        ProtonMass(mpMKS/unitMkg),
        ElectronMass(meMKS/unitMkg),
        ElectronCharge(qeMKS / unitCcou),
        GGravity(GMKS/(unitLm/unitMkg*pow(unitLm/unitTsec,2.0))),
        cLight(cMKS/(unitLm/unitTsec)),
        kBoltzmann(kBMKS*unitTeK/(unitMkg*pow(unitLm/unitTsec,2.0))),
        MolarGasConstant(RgasMKS*unitTeK/(unitMkg*pow(unitLm/unitTsec,2.0))),
        KelvinsToEnergyPerMole(unitMkg*pow(unitLm/unitTsec,2.0)/(kBMKS*unitTeK)*NAvogadro),
        UnitMassDensity(unitMkg/(unitLm*unitLm*unitLm)),
        Sigma(StefanBoltzmannMKS*unitTeK*unitTeK*unitTeK*unitTeK/unitMkg*unitTsec*unitTsec*unitTsec),
        BlackBody(4*StefanBoltzmannMKS*unitTeK*unitTeK*unitTeK*unitTeK/cMKS*unitTsec*unitTsec*unitLm/unitMkg),
        Planck(PlanckMKS*unitTsec/(unitMkg*unitLm*unitLm)),
        EpsilonZero(EpsilonZeroMKS*pow(unitLm,3.0)*unitMkg/(pow(unitTsec,2.0)*pow(unitCcou,2.0))) { }


    
    // The fundamental independent quantities.
    double unitLengthMeters() const { return mUnitLm; }
    double unitMassKg() const { return mUnitMkg; }
    double unitTimeSec() const { return mUnitTsec; }
    double unitTemperatureKelvin() const { return mUnitTeK; }
    double unitChargeCoulomb() const { return mUnitCcou; }

    // All the stuff we provide.
    double protonMass() const { return ProtonMass; }
    double electronMass() const { return ElectronMass; }
    double electronCharge() const { return ElectronCharge; }
    double G() const { return GGravity; }
    double c() const { return cLight; }
    double kB() const { return kBoltzmann; }
    double molarGasConstant() const { return MolarGasConstant; }
    double kelvinsToEnergyPerMole() const { return KelvinsToEnergyPerMole; }
    double unitMassDensity() const { return UnitMassDensity; }
    double stefanBoltzmannConstant() const { return Sigma; }
    double blackBodyConstant() const { return BlackBody; }
    double planckConstant() const { return Planck; }
    double unitEnergyJ() const { return UnitEnergyJ; }
    double epsilonZero() const { return EpsilonZero; }

};

#endif // PHYSICALCONSTANTS_HH