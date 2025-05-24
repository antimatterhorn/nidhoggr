#include "physics.hh"
#include "../EOS/equationOfState.hh"
#include "../Mesh/grid.hh"

template <int dim>
class ThermalConduction : public Physics<dim> {
protected:
    Mesh::Grid<dim>* grid;
    EquationOfState* eos;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    ThermalConduction(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Physics<dim>(nodeList,constants), eos(eos), grid(grid) {
        VerifyFields(nodeList);
    }

    virtual ~ThermalConduction() {}

    virtual void
    VerifyFields(NodeList* nodeList) {
        this->template EnrollFields<double>({"pressure", "density", "specificInternalEnergy", "soundSpeed"});
        this->template EnrollStateFields<double>({"specificInternalEnergy"});
    }

    virtual void PreStepInitialize() override {
        this->state.updateFields(this->nodeList);
    }

    virtual void EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        NodeList* nodeList = this->nodeList;
        int numZones = nodeList->size();

        ScalarField* rho    = nodeList->getField<double>("density");
        ScalarField* u      = initialState->template getField<double>("specificInternalEnergy");

        ScalarField* dudt   = deriv.template getField<Vector>("specificInternalEnergy");

        for (int i = 0 ; i < numZones ; ++i) {
            if (!grid->onBoundary(i)) {
                // const int iL = grid->left(i);
                // const int iR = grid->right(i);

                // const double dxL = grid->dx(iL, i);
                // const double dxR = grid->dx(i, iR);

                // const double gradT_L = (T[i] - T[iL]) / dxL;
                // const double gradT_R = (T[iR] - T[i]) / dxR;

                // const double flux_L = -kappa[i] * gradT_L;
                // const double flux_R = -kappa[i] * gradT_R;

                // const double volume = grid->cellVolume(i);
                // du_dt[i] = (flux_L - flux_R) / volume;
            }
        }

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