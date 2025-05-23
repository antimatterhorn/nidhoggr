#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include "equationOfState.hh"

class HelmholtzEOS : public EquationOfState {
private:
    std::vector<double> logRhoGrid;
    std::vector<double> logTGrid;

    std::vector<std::vector<double>> PTable;
    std::vector<std::vector<double>> UTable;
    std::vector<std::vector<double>> CsTable;

    const PhysicalConstants constants;

    size_t findIndex(const std::vector<double>& grid, double value) const {
        auto it = std::upper_bound(grid.begin(), grid.end(), value);
        if (it == grid.begin() || it == grid.end()) return std::max<size_t>(grid.size() - 2, 0);
        return std::distance(grid.begin(), it) - 1;
    }
    
    double bilinearInterp(const std::vector<std::vector<double>>& table,
                          double logRho, double logT) const {
        size_t i = findIndex(logRhoGrid, logRho);
        size_t j = findIndex(logTGrid, logT);

        double x0 = logRhoGrid[i], x1 = logRhoGrid[i + 1];
        double y0 = logTGrid[j],   y1 = logTGrid[j + 1];

        double q11 = table[i][j];
        double q12 = table[i][j + 1];
        double q21 = table[i + 1][j];
        double q22 = table[i + 1][j + 1];

        double tx = (logRho - x0) / (x1 - x0);
        double ty = (logT - y0) / (y1 - y0);

        return (1 - tx) * (1 - ty) * q11 +
               (1 - tx) * ty       * q12 +
               tx       * (1 - ty) * q21 +
               tx       * ty       * q22;
    }

    
    void loadTable(const std::string& filename) {
        FILE* file = fopen(filename.c_str(), "r");
        if (!file) throw std::runtime_error("Failed to open EOS table: " + filename);

        char line[512];
        std::vector<std::tuple<double, double, double, double, double>> rows;
        std::vector<double> rawRhos, rawTs;

        while (fgets(line, sizeof(line), file)) {
            double col[9];
            int count = sscanf(line, "%lf %lf %lf %lf %lf %lf %lf %lf %lf",
                            &col[0], &col[1], &col[2], &col[3], &col[4],
                            &col[5], &col[6], &col[7], &col[8]);
            if (count != 9) continue;

            double logRho = std::log10(std::abs(col[0]));
            double logT   = std::log10(std::abs(col[1]));
            double logP   = std::log10(std::abs(col[2]));
            double logU   = std::log10(std::abs(col[3]));
            double logCs  = std::log10(std::abs(col[7]));

            rows.emplace_back(logRho, logT, logP, logU, logCs);
            rawRhos.push_back(logRho);
            rawTs.push_back(logT);
        }
        fclose(file);

        std::sort(rawRhos.begin(), rawRhos.end());
        std::sort(rawTs.begin(), rawTs.end());
        rawRhos.erase(std::unique(rawRhos.begin(), rawRhos.end()), rawRhos.end());
        rawTs.erase(std::unique(rawTs.begin(), rawTs.end()), rawTs.end());

        logRhoGrid = std::move(rawRhos);
        logTGrid   = std::move(rawTs);

        const size_t nRho = logRhoGrid.size();
        const size_t nT   = logTGrid.size();

        PTable.resize(nRho, std::vector<double>(nT));
        UTable.resize(nRho, std::vector<double>(nT));
        CsTable.resize(nRho, std::vector<double>(nT));

        for (const auto& [lrho, lT, lP, lU, lCs] : rows) {
            auto i = std::distance(logRhoGrid.begin(), std::find(logRhoGrid.begin(), logRhoGrid.end(), lrho));
            auto j = std::distance(logTGrid.begin(), std::find(logTGrid.begin(), logTGrid.end(), lT));
            PTable[i][j] = std::pow(10.0, lP);
            UTable[i][j] = std::pow(10.0, lU);
            CsTable[i][j] = std::pow(10.0, lCs);
        }
    }

public:
    HelmholtzEOS(const std::string& tableFile, PhysicalConstants& constants) : 
        EquationOfState(constants), constants(constants) {}

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
        throw std::runtime_error("not implemented yet.");
    }
    virtual void 
    setInternalEnergyFromTemperature(Field<double>* internalEnergy, Field<double>* density, Field<double>* temperature) const override {
        throw std::runtime_error("not implemented yet.");
    }

    virtual void 
    setPressure(double* pressure, double* density, double* internalEnergy) const override {
        throw std::runtime_error("not implemented yet.");
    }

    virtual void 
    setInternalEnergy(double* internalEnergy, double* density, double* pressure) const override {
        throw std::runtime_error("not implemented yet.");
    }

    virtual void 
    setSoundSpeed(double* soundSpeed, double* density,double* internalEnergy) const override {
        throw std::runtime_error("not implemented yet.");
    }

    virtual void 
    setTemperature(double* temperature, double* density, double* internalEnergy) const override {
        throw std::runtime_error("not implemented yet.");
    }

    virtual void 
    setInternalEnergyFromTemperature(double* internalEnergy, double* density, double* temperature) const override {
        throw std::runtime_error("not implemented yet.");
    }


    virtual double 
    getGamma() const override { return 0; };

    virtual std::string name() const override {
        return "HelmholtzEOS";
    }

};

HelmholtzEOS::~HelmholtzEOS() = default;