#include "hydro.hh"
#include "../Mesh/grid.hh"
#include "HLL.cc"
#include <iostream>

template <int dim>
class GridHydroHLLE : public Hydro<dim> {
protected:
    Mesh::Grid<dim>* grid;
    std::vector<int> insideIds;
    double dtmin, dxmin;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    GridHydroHLLE(NodeList* nodeList, PhysicalConstants& constants, EquationOfState* eos, Mesh::Grid<dim>* grid) : 
        Hydro<dim>(nodeList, constants, eos), grid(grid) {
        std::cout << grid->size() << std::endl;
        VerifyHLLFields(nodeList);

        State<dim>* state = &this->state;

        VectorField* v            = nodeList->getField<Vector>("velocity");
        ScalarField* rho          = nodeList->getField<double>("density");
        ScalarField* u            = nodeList->getField<double>("specificInternalEnergy");

        state->template addField<Vector>(v);
        state->template addField<double>(rho);
        state->template addField<double>(u);

        // ScalarField* pressure   = nodeList->getField<double>("pressure");
        // ScalarField* soundSpeed = nodeList->getField<double>("soundSpeed");

        for (int i = 0; i < grid->size(); i++) {
            if (!grid->onBoundary(i))
                insideIds.push_back(i);
        }

        dxmin = 1e30;
        for (int i = 0; i<dim; i++)
            dxmin = std::min(dxmin,grid->spacing(i));
    }

    ~GridHydroHLLE() {}

    virtual void 
    ZeroTimeInitialize() override {
        EOSLookup();
    }

    virtual void 
    VerifyHLLFields(NodeList* nodeList) {
        grid->assignPositions(nodeList);
    }

    virtual void 
    EvaluateDerivatives(const State<dim>* initialState,
            State<dim>& deriv,
            const double time,
            const double dt) override {
        NodeList* nodeList = this->nodeList;

        EOSLookup();
        dtmin = 1e30;
        // Primitive state fields
        auto* rho = initialState->template getField<double>("density");
        auto* v   = initialState->template getField<Vector>("velocity");
        auto* u   = initialState->template getField<double>("specificInternalEnergy");

        // Derivative fields
        auto* drhodt = deriv.template getField<double>("density");
        auto* dvdt   = deriv.template getField<Vector>("velocity");
        auto* dudt   = deriv.template getField<double>("specificInternalEnergy");

        // Derived fields
        auto* pressure   = nodeList->getField<double>("pressure");
        auto* soundSpeed = nodeList->getField<double>("soundSpeed");

        #pragma omp parallel for
        for (int h = 0; h < insideIds.size(); ++h) {
            int i = insideIds[h];

            Vector vi = v->getValue(i);
            double rhoi = rho->getValue(i);
            double ui   = u->getValue(i);
            double Pi   = pressure->getValue(i);
            double ci   = soundSpeed->getValue(i);

            if (!std::isfinite(rhoi) || !std::isfinite(ui) || !std::isfinite(vi[0]) || !std::isfinite(vi[1]))
                std::cerr << "BAD STATE at cell " << i << ": rho=" << rhoi << ", u=" << ui << ", v=" << vi.toString() << std::endl;


            double ei = ui + 0.5 * vi.mag2();     // total specific energy
            Vector momi = vi * rhoi;              // momentum
            double Ei = rhoi * ei;                // total energy

            // Net flux accumulators
            double net_rho_flux = 0.0;
            Vector net_mom_flux = Vector::zero();
            double net_E_flux   = 0.0;

            auto neighbors = grid->getNeighboringCells(i);

            for (int k = 0; k < dim; ++k) {
                int jL = neighbors[2 * k];       // left neighbor in dimension k
                int jR = neighbors[2 * k + 1];   // right neighbor in dimension k

                auto flux_L = computeHLLFlux<dim>(jL, i, k, *rho, *v, *u, *pressure, *soundSpeed);
                auto flux_R = computeHLLFlux<dim>(i, jR, k, *rho, *v, *u, *pressure, *soundSpeed);

                double dx = grid->spacing(k);

                net_rho_flux += (flux_L.mass - flux_R.mass) / dx;
                net_mom_flux += (flux_L.momentum - flux_R.momentum) / dx;
                net_E_flux   += (flux_L.energy - flux_R.energy) / dx;
            }

            // Derivative updates
            drhodt->setValue(i, net_rho_flux);

            Vector dvi = (net_mom_flux - vi * net_rho_flux) / rhoi;
            double dui = (net_E_flux
                - vi.dot(net_mom_flux)
                - 0.5 * vi.mag2() * net_rho_flux) / rhoi;

            dvdt->setValue(i, dvi);
            dudt->setValue(i, dui);

            dtmin = std::min(dtmin,0.4*dxmin/ci);
        }
    }

    virtual void 
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;
        ScalarField* fdensity       = finalState->template getField<double>("density");
        VectorField* fvelocity      = finalState->template getField<Lin::Vector<dim>>("velocity");
        ScalarField* fu             = finalState->template getField<double>("specificInternalEnergy");

        ScalarField* density        = nodeList->getField<double>("density");
        VectorField* velocity       = nodeList->getField<Lin::Vector<dim>>("velocity");
        ScalarField* u              = nodeList->getField<double>("specificInternalEnergy");

        density->copyValues(fdensity);
        velocity->copyValues(fvelocity);
        u->copyValues(fu);

        #pragma omp parallel for
        for(int i = 0 ; i < nodeList->size() ; ++i) {
            density->setValue(i,std::max(density->getValue(i),1e-12));
            u->setValue(i,std::max(u->getValue(i),1e-12));
        }

        EOSLookup();
    }

    virtual double EstimateTimestep() const override {
        return dtmin;
    }

    virtual void 
    EOSLookup() {        
        NodeList* nodeList = this->nodeList;

        ScalarField* rho        = nodeList->getField<double>("density");
        ScalarField* u          = nodeList->getField<double>("specificInternalEnergy");
        ScalarField* pressure   = nodeList->getField<double>("pressure");
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

    virtual std::string name() const override { return "gridHydroHLL"; }
    virtual std::string description() const override {
        return "The HLL solver for hydro"; }
};
