#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include "equationOfState.hh"
#include "../Math/scalarMath.hh"

class HelmholtzEOS : public EquationOfState {
private:
    std::vector<double> logRhoGrid;
    std::vector<double> logUGrid;

    std::vector<std::vector<double>> PTable;
    std::vector<std::vector<double>> UTable;
    std::vector<std::vector<double>> CsTable;

    const PhysicalConstants constants;
    const std::string& tableFile;

    size_t findIndex(const std::vector<double>& grid, double value) const {
        auto it = std::upper_bound(grid.begin(), grid.end(), value);
        if (it == grid.begin() || it == grid.end()) return std::max<size_t>(grid.size() - 2, 0);
        return std::distance(grid.begin(), it) - 1;
    }
    
    double bilinearInterp(const std::vector<std::vector<double>>& table,
                          double logRho, double logU) const {
        size_t i = findIndex(logRhoGrid, logRho);
        size_t j = findIndex(logUGrid, logU);

        double x0 = logRhoGrid[i], x1 = logRhoGrid[i + 1];
        double y0 = logUGrid[j],   y1 = logUGrid[j + 1];

        double q11 = table[i][j];
        double q12 = table[i][j + 1];
        double q21 = table[i + 1][j];
        double q22 = table[i + 1][j + 1];

        double tx = (logRho - x0) / (x1 - x0);
        double ty = (logU - y0) / (y1 - y0);

        return (1 - tx) * (1 - ty) * q11 +
               (1 - tx) * ty       * q12 +
               tx       * (1 - ty) * q21 +
               tx       * ty       * q22;
    }

    
    void loadTable(const std::string& filename) {
        FILE* file = std::fopen(filename.c_str(), "r");
        if (!file) {
            throw std::runtime_error("Failed to open EOS table: " + filename);
        }

        char line[512];
        std::vector<std::tuple<double, double, double, double>> rows;
        std::vector<double> rawRhos, rawUs;

        while (std::fgets(line, sizeof(line), file)) {
            double logRho, logU, P, cs;
            int count = std::sscanf(line, "%lf %lf %lf %lf", &logRho, &logU, &P, &cs);
            if (count != 4) continue;

            rows.emplace_back(logRho, logU, P, cs);
            rawRhos.push_back(logRho);
            rawUs.push_back(logU);
        }
        std::fclose(file);

        // Deduplicate and sort axes
        std::sort(rawRhos.begin(), rawRhos.end());
        std::sort(rawUs.begin(), rawUs.end());
        rawRhos.erase(std::unique(rawRhos.begin(), rawRhos.end()), rawRhos.end());
        rawUs.erase(std::unique(rawUs.begin(), rawUs.end()), rawUs.end());

        logRhoGrid = std::move(rawRhos);
        logUGrid   = std::move(rawUs);

        const size_t nRho = logRhoGrid.size();
        const size_t nU   = logUGrid.size();

        PTable.resize(nRho, std::vector<double>(nU));
        UTable.resize(nRho, std::vector<double>(nU));
        CsTable.resize(nRho, std::vector<double>(nU));

        for (const auto& [logRho, logU, P, cs] : rows) {
            size_t i = std::distance(logRhoGrid.begin(), std::find(logRhoGrid.begin(), logRhoGrid.end(), logRho));
            size_t j = std::distance(logUGrid.begin(), std::find(logUGrid.begin(), logUGrid.end(), logU));
            PTable[i][j]  = P;
            UTable[i][j]  = std::pow(10.0, logU);  // Store internal energy in linear space
            CsTable[i][j] = cs;
        }
    }


    void computeHelmholtzApprox(double rho, double u, double& P, double& cs) {
        const double kB = constants.kB();            // erg/K
        const double mH = constants.protonMass();    // g
        const double mu = 0.6;                       // mean molecular weight

        // Convert internal energy per mass to temperature (ideal gas approx)
        double T = (2.0 / 3.0) * (mu * mH / kB) * u;

        // Ideal ion pressure
        double P_ion = (rho / (mu * mH)) * kB * T;

        // Degenerate electron pressure estimate (non-relativistic)
        constexpr double mu_e = 2.0;
        double rho_over_me = rho / (mu_e * mH);
        double P_deg = 1.0e13 * std::pow(rho_over_me, 5.0 / 3.0);

        // Total pressure and sound speed
        P = P_ion + P_deg;
        double gamma_eff = 5.0 / 3.0;
        cs = std::sqrt(gamma_eff * P / rho);
    }

public:
    HelmholtzEOS(const std::string& tableFile, PhysicalConstants& constants) : 
        EquationOfState(constants), constants(constants), tableFile(tableFile) {
        FILE* fp = std::fopen(tableFile.c_str(), "r");
        if (fp) {
            std::fclose(fp);
            std::cout << "Using table file: " << tableFile << std::endl;
            loadTable(tableFile);
        } else {
            std::cout << "Generating table file: " << tableFile << std::endl;
            generateTable();
        }
    }

    virtual ~HelmholtzEOS();

    virtual void 
    setPressure(Field<double>* pressure, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < pressure->size(); ++i) {
            double logRho = std::log10(density->getValue(i));
            double logU   = std::log10(internalEnergy->getValue(i));
            pressure->setValue(i, bilinearInterp(PTable, logRho, logU));
        }
    }
    virtual void 
    setInternalEnergy(Field<double>* internalEnergy, Field<double>* density, Field<double>* pressure) const override {
        for (int i = 0; i < internalEnergy->size(); ++i) {
            double logRho = std::log10(density->getValue(i));
            double logP   = std::log10(pressure->getValue(i)); // Placeholder
            internalEnergy->setValue(i, bilinearInterp(UTable, logRho, logP));
        }
    }
    virtual void 
    setSoundSpeed(Field<double>* soundSpeed, Field<double>* density, Field<double>* internalEnergy) const override {
        for (int i = 0; i < soundSpeed->size(); ++i) {
            double logRho = std::log10(density->getValue(i));
            double logU   = std::log10(internalEnergy->getValue(i));
            soundSpeed->setValue(i, bilinearInterp(CsTable, logRho, logU));
        }
    }
    virtual void 
    setTemperature(Field<double>* temperature, Field<double>* density, Field<double>* internalEnergy) const override {
        const double kB = constants.kB();             // erg/K in code units
        const double mH = constants.protonMass();     // g in code units
        constexpr double mu = 0.6;                    // Mean molecular weight (helium-rich plasma) fix this later

        for (int i = 0; i < temperature->size(); ++i) {
            double u = internalEnergy->getValue(i);
            double T = (2.0 / 3.0) * (mu * mH / kB) * u;
            temperature->setValue(i, T);
        }
    }

    virtual void 
    setInternalEnergyFromTemperature(Field<double>* internalEnergy, Field<double>* density, Field<double>* temperature) const override {
        const double kB = constants.kB();             // erg/K in code units
        const double mH = constants.protonMass();     // g in code units
        constexpr double mu = 0.6;                    // Mean molecular weight

        for (int i = 0; i < internalEnergy->size(); ++i) {
            double T = temperature->getValue(i);
            double u = (3.0 / 2.0) * (kB / (mu * mH)) * T;
            internalEnergy->setValue(i, u);
        }
    }


    virtual void 
    setPressure(double* pressure, double* density, double* internalEnergy) const override {
        double logRho = std::log10(*density);
        double logU   = std::log10(*internalEnergy);
        *pressure = bilinearInterp(PTable, logRho, logU);
    }

    virtual void 
    setInternalEnergy(double* internalEnergy, double* density, double* pressure) const override {
        double logRho = std::log10(*density);
        double logP   = std::log10(*pressure);  // approximate inverse
        *internalEnergy = bilinearInterp(UTable, logRho, logP);
    }

    virtual void 
    setSoundSpeed(double* soundSpeed, double* density, double* internalEnergy) const override {
        double logRho = std::log10(*density);
        double logU   = std::log10(*internalEnergy);
        *soundSpeed = bilinearInterp(CsTable, logRho, logU);
    }

    virtual void 
    setTemperature(double* temperature, double* density, double* internalEnergy) const override {
        const double kB = constants.kB();             // erg/K in code units
        const double mH = constants.protonMass();     // g in code units
        constexpr double mu = 0.6;                    // mean molecular weight

        *temperature = (2.0 / 3.0) * (mu * mH / kB) * (*internalEnergy);
    }

    virtual void 
    setInternalEnergyFromTemperature(double* internalEnergy, double* density, double* temperature) const override {
        const double kB = constants.kB();             // erg/K in code units
        const double mH = constants.protonMass();     // g in code units
        constexpr double mu = 0.6;                    // mean molecular weight

        *internalEnergy = (3.0 / 2.0) * (kB / (mu * mH)) * (*temperature);
    }


    virtual std::string name() const override {
        return "HelmholtzEOS";
    }

    void generateTable() {
        // Define log-spaced rho and u values
        logRhoGrid = logspace(-2, 6, 200);  // log10(rho) from 1e-2 to 1e6
        logUGrid   = logspace(-2, 8, 200);  // log10(u) from 1e-2 to 1e8

        // Resize tables
        PTable.resize(logRhoGrid.size(), std::vector<double>(logUGrid.size()));
        UTable = PTable;
        CsTable = PTable;

        for (size_t i = 0; i < logRhoGrid.size(); ++i) {
            for (size_t j = 0; j < logUGrid.size(); ++j) {
                double rho = std::pow(10.0, logRhoGrid[i]);
                double u   = std::pow(10.0, logUGrid[j]);

                double P, cs;
                computeHelmholtzApprox(rho, u, P, cs);

                PTable[i][j]  = P;
                UTable[i][j]  = u;
                CsTable[i][j] = cs;
            }
        }

        // Write to file using C stdio
        FILE* fp = std::fopen(tableFile.c_str(), "w");
        if (!fp) throw std::runtime_error("Could not open file for writing: " + tableFile);

        for (size_t i = 0; i < logRhoGrid.size(); ++i) {
            for (size_t j = 0; j < logUGrid.size(); ++j) {
                std::fprintf(fp, "%.10e %.10e %.10e %.10e\n",
                            logRhoGrid[i], logUGrid[j],
                            PTable[i][j], CsTable[i][j]);
            }
        }

        std::fclose(fp);
    }

};

HelmholtzEOS::~HelmholtzEOS() = default;