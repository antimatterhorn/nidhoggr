#include "hydro.hh"
#include "../Mesh/grid.hh"
#include <iostream>

template <int dim>
class GridHydroHLL : public Hydro<dim> {
protected:
    Mesh::Grid<dim>* grid;
    std::vector<int> insideIds;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;
    
    GridHydroHLL() {}

    GridHydroHLL(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList, constants, eos), grid(grid) {
        std::cout << grid->size() << std::endl;
        VerifyHLLFields(nodeList);

        State<dim>* state = &this->state;

        VectorField* v  = nodeList->getField<Vector>("velocity");
        ScalarField* rho          = nodeList->getField<double>("density");
        ScalarField* u            = nodeList->getField<double>("specificInternalEnergy");

        state->template addField<Vector>(v);
        state->template addField<double>(rho);
        state->template addField<double>(u);

        for (int i = 0; i < grid->size(); i++) {
            if (!grid->onBoundary(i))
                insideIds.push_back(i);
        }
    }

    ~GridHydroHLL() {}

    virtual void 
    ZeroTimeInitialize() override {
        EOSLookup();
    }

    virtual void 
    VerifyHLLFields(NodeList* nodeList) {
        if (nodeList->getField<Vector>("position") == nullptr)
            nodeList->insertField<Vector>("position");
                
        VectorField* position = nodeList->getField<Vector>("position");
        for (int i = 0; i < position->size(); ++i) {
            position->setValue(i, grid->getPosition(i));
        }
    }

    virtual void 
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        VectorField F0("f0", initialState->size());
        VectorField F1("f1", initialState->size());
        VectorField F2("f2", initialState->size());

        VectorField u1("u1", initialState->size());
        ScalarField u0("u0", initialState->size());
        ScalarField u2("u2", initialState->size());

        VectorField* v   = initialState->template getField<Vector>("velocity");
        ScalarField* rho = initialState->template getField<double>("density");
        ScalarField* u   = initialState->template getField<double>("specificInternalEnergy");

        VectorField* dvdt = deriv.template getField<Vector>("velocity");
        ScalarField* drdt = deriv.template getField<double>("density");
        ScalarField* dudt = deriv.template getField<double>("specificInternalEnergy");

        ScalarField* pressure   = nodeList->getField<double>("pressure");
        ScalarField* soundSpeed = nodeList->getField<double>("soundSpeed");

        std::vector<VectorField*> F = {&F0, &F1, &F2};

        VectorField ep("ep", initialState->size());
        VectorField em("em", initialState->size());

        for (int h = 0; h < insideIds.size(); ++h) {
            int i = insideIds[h];

        }

    }

    std::array<double, 3> 
    getFlux(int axis, int i, int j, const State<dim>* initialState, 
        VectorField& ep, VectorField& em, 
        std::vector<VectorField*>& F) {
        double epR, epL, emR, emL, ap, am;
        epR = 0.5 * (ep.getValue(j)[axis] + ep.getValue(i)[axis]);
        epL = ep.getValue(i)[axis];
        emR = 0.5 * (em.getValue(j)[axis] + em.getValue(i)[axis]);
        emL = em.getValue(i)[axis];

        ap = std::max({epL, epR, 0.0});
        am = std::max({-emL, -emR, 0.0}); 

        std::array<double, 3> FL, FR, Flux;

        double u0L, u0R, u2L, u2R;
        Vector u1L, u1R;

        VectorField* u1 = initialState->template getField<Vector>("u1");
        Field<double>* u0 = initialState->template getField<double>("u0");
        Field<double>* u2 = initialState->template getField<double>("u2");

        u0L = u0->getValue(i);
        u0R = 0.5 * (u0->getValue(j) + u0->getValue(i));
        u1L = u1->getValue(i);
        u1R = 0.5 * (u1->getValue(j) + u1->getValue(i));
        u2L = u2->getValue(i);
        u2R = 0.5 * (u2->getValue(j) + u2->getValue(i));

        for (int k = 0; k < 3; ++k) {
            FL[k] = F[k]->getValue(i)[axis];
            FR[k] = 0.5 * (F[k]->getValue(i)[axis] + F[k]->getValue(j)[axis]);
        }

        Flux[0] = (ap * FL[0] + am * FR[0] - ap * am * (u0R - u0L)) / (ap + am); 
        Flux[1] = (ap * FL[1] + am * FR[1] - ap * am * (u1R - u1L)[axis]) / (ap + am); 
        Flux[2] = (ap * FL[2] + am * FR[2] - ap * am * (u2R - u2L)) / (ap + am);

        return Flux;
    }

    virtual void 
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;

        EOSLookup();
    }

    virtual void 
    EOSLookup() {        
        NodeList* nodeList = this->nodeList;

        ScalarField* rho = nodeList->getField<double>("density");
        ScalarField* u = nodeList->getField<double>("specificInternalEnergy");
        ScalarField* pressure = nodeList->getField<double>("pressure");
        ScalarField* soundSpeed = nodeList->getField<double>("soundSpeed");

        EquationOfState* eos = this->eos;
        eos->setPressure(pressure, rho, u);
        eos->setSoundSpeed(soundSpeed, rho, u);
    }

    double 
    getCell(int i, int j, std::string fieldName = "pressure") {
        int idx = grid->index(i, j, 0);
        NodeList* nodeList = this->nodeList;
        ScalarField* pr = nodeList->getField<double>(fieldName);
        return pr->getValue(idx);
    }
};
