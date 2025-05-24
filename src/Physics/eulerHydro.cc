#include "hydro.hh"
#include "../Mesh/grid.hh"
#include <iostream>

template <int dim>
class EulerHydro : public Hydro<dim> {
protected:
    Mesh::Grid<dim>* grid;
    std::vector<int> insideIds;
public:
    EulerHydro() {}

    EulerHydro(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList,constants,eos), grid(grid){
        this->EnrollVectors({"position"});

        this->EnrollStateScalars({"density", "specificInternalEnergy"});
        this->EnrollStateVectors({"velocity"});

        for(int i=0;i<grid->size();i++)
            if(!grid->onBoundary(i))
                insideIds.push_back(i);

        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        for(int i=0;i<position->size();++i)
            position->setValue(i,grid->getPosition(i));
    }

    ~EulerHydro() {}

    virtual void
    ZeroTimeInitialize() override {      
        NodeList* nodeList = this->nodeList;
        Field<Lin::Vector<dim>>* v  = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* rho          = nodeList->getField<double>("density");
        Field<double>* u            = nodeList->getField<double>("specificInternalEnergy");

        EOSLookup();
    }

    virtual void EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* velocity = initialState->template getField<Lin::Vector<dim>>("velocity");
        Field<double>* density = initialState->template getField<double>("density");
        Field<double>* drhodt = deriv.template getField<double>("density");
        Field<double>* dudt = deriv.template getField<double>("specificInternalEnergy");
        Field<Lin::Vector<dim>>* dvdt = deriv.template getField<Lin::Vector<dim>>("velocity");
        Field<double>* pressure = nodeList->getField<double>("pressure");
        Field<double>* soundSpeed = nodeList->getField<double>("soundSpeed");

        double artificialViscosityCoefficient = 0.1; // Adjust as needed

        for (int h = 0; h < insideIds.size(); ++h) {
            int i = insideIds[h];
            std::vector<int> nbrs = grid->getNeighboringCells(i);

            double delDotV = 0.0;
            Lin::Vector<dim> delP = Lin::Vector<dim>();
            Lin::Vector<dim> vDelv = Lin::Vector<dim>();

            // Compute divergence of velocity
            for (int d = 0; d < dim; ++d) {
                double dvdx = (velocity->getValue(nbrs[2 * d])[d] - velocity->getValue(i)[d]) +
                            (velocity->getValue(i)[d] - velocity->getValue(nbrs[2 * d + 1])[d]);
                dvdx = dvdx * 0.5 / grid->spacing(d);
                delDotV += dvdx;
                vDelv[d] = velocity->getValue(i)[d]*dvdx;

                // Compute pressure gradient
                double dPdx = (pressure->getValue(nbrs[2 * d]) - pressure->getValue(i)) +
                            (pressure->getValue(i) - pressure->getValue(nbrs[2 * d + 1]));
                dPdx = dPdx * 0.5 / grid->spacing(d);
                delP[d] = dPdx;

                // Add artificial viscosity
                double velocityDifference = velocity->getValue(nbrs[2 * d])[d] - velocity->getValue(nbrs[2 * d + 1])[d];
                delP[d] += artificialViscosityCoefficient * std::abs(velocityDifference) / grid->spacing(d);
            }

            double P = pressure->getValue(i);
            double rho = density->getValue(i);
            drhodt->setValue(i, -rho * delDotV);
            dvdt->setValue(i, -delP / rho - vDelv);
            dudt->setValue(i, -P / rho * delDotV);
        }
    }


    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;

        Field<double>* fdensity                 = finalState->template getField<double>("density");
        Field<Lin::Vector<dim>>* fvelocity      = finalState->template getField<Lin::Vector<dim>>("velocity");
        Field<double>* fu                       = finalState->template getField<double>("specificInternalEnergy");

        Field<double>* density                  = nodeList->getField<double>("density");
        Field<Lin::Vector<dim>>* velocity       = nodeList->getField<Lin::Vector<dim>>("velocity");
        Field<double>* u                        = nodeList->getField<double>("specificInternalEnergy");

        density->copyValues(fdensity);
        velocity->copyValues(fvelocity);
        u->copyValues(fu);

        EOSLookup();
    }

    virtual void
    EOSLookup() {        
        NodeList* nodeList = this->nodeList;

        Field<double>* rho          = nodeList->getField<double>("density");
        Field<double>* u            = nodeList->getField<double>("specificInternalEnergy");
        Field<double>* pressure     = nodeList->getField<double>("pressure");
        Field<double>* soundSpeed   = nodeList->getField<double>("soundSpeed");

        EquationOfState* eos = this->eos;
        eos->setPressure(pressure,rho,u);
        eos->setSoundSpeed(soundSpeed,rho,u);
    }

    double
    getCell(int i,int j, std::string fieldName="pressure") {
        int idx = grid->index(i,j,0);
        NodeList* nodeList = this->nodeList;
        Field<double>* pr = nodeList->getField<double>(fieldName);
        return pr->getValue(idx);
    }


    virtual double 
    EstimateTimestep() const override {
        // dt = dh/vmax
        return 0; 
    }
};
