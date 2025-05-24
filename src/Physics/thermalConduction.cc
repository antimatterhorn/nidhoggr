#include "physics.hh"
#include "../EOS/equationOfState.hh"
#include "../Mesh/grid.hh"

template <int dim>
class ThermalConduction : public Physics<dim> {
protected:
    Mesh::Grid<dim>* grid;
    EquationOfState* eos;
public:

    Hydro(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Physics<dim>(nodeList,constants), eos(eos), grid(grid) {
        VerifyHydroFields(nodeList);
    }

    virtual ~Hydro() {}

    virtual void
    VerifyHydroFields(NodeList* nodeList) {
        for (const std::string& name : 
            {"pressure", "density", "specificInternalEnergy", "soundSpeed"}) {
            if (nodeList->getField<double>(name) == nullptr)
                nodeList->insertField<double>(name);
        }
    }

    virtual void PreStepInitialize() override {
        this->state.updateFields(this->nodeList);
    }

    virtual void EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        NodeList* nodeList = this->nodeList;
        int numZones = nodeList->size();

        this->lastDt = dt;
    }

    virtual double EstimateTimestep() const override {
        return 1e30; // this physics package does not support setting the timestep for now
    }

    virtual void FinalizeStep(const State<dim>* finalState) override {
        PushState(finalState);
    }

    virtual void PushState(const State<dim>* stateToPush) override {
        NodeList* nodeList = this->nodeList;
        int numZones = nodeList->size();
        State<dim> state = this->state;

    }

    virtual std::string name() const override { return "ThermalConduction"; }
    virtual std::string description() const override {
        return "Thermal conduction physics"; }
};