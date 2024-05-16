#include "physics.hh"
#include "../Mesh/grid.hh"
#include <iostream>

template <int dim>
class WaveEquation : public Physics<dim> {
protected:
    Mesh::Grid<dim>* grid;
    double C;
public:
    WaveEquation() {}

    WaveEquation(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<dim>* grid, double C) : 
        Physics<dim>(nodeList,constants),
        grid(grid), C(C) {

        int numNodes = nodeList->size();
        if (nodeList->getField<double>("phi") == nullptr)
            nodeList->insertField<double>("phi");
        if (nodeList->getField<double>("xi") == nullptr)
            nodeList->insertField<double>("xi");
        
        State<dim>* state = &this->state;
        Field<double>* xi = nodeList->getField<double>("xi");
        //std::shared_ptr<Field<double>> xiSharedPtr(xi);
        state->template addField<double>(xi);
        Field<double>* phi = nodeList->getField<double>("phi");
        //std::shared_ptr<Field<double>> phiSharedPtr(phi);
        state->template addField<double>(phi);
    }

    ~WaveEquation() {}

    virtual void
    PreStepInitialize() override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;
        state.updateFields(nodeList);
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double t) override {  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        
        Field<double>* xi = initialState->template getField<double>("xi");
        Field<double>* phi = initialState->template getField<double>("phi");

        Field<double>* dxi = deriv.template getField<double>("xi");
        Field<double>* dphi = deriv.template getField<double>("phi");
        
        #pragma omp parallel for
        for (int i=0; i<numNodes; ++i) {
            std::vector<int> neighbors = grid->getNeighboringCells(i);
            double laplace2 = -4*phi->getValue(i);
            for (auto idx : neighbors) {
                laplace2 += phi->getValue(idx);
            }                
            laplace2 = laplace2/pow(grid->dx,2.0);
            dxi->setValue(i,laplace2*C*C); 
            dphi->setValue(i,t*dxi->getValue(i)+xi->getValue(i));         
        }
    }

    double
    getCell(int i,int j) {
        int idx = grid->index(i,j,0);
        NodeList* nodeList = this->nodeList;
        Field<double>* phi = nodeList->getField<double>("phi");
        return phi->getValue(idx);
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        Field<double>* fxi = finalState->template getField<double>("xi");
        Field<double>* fphi = finalState->template getField<double>("phi");

        Field<double>* xi = nodeList->getField<double>("xi");
        Field<double>* phi = nodeList->getField<double>("phi");

        xi->copyValues(fxi);
        phi->copyValues(fphi);
    }


// d^2 phi / dt^2 = c^2 del^2 phi

// IIRC, which solution you get all comes down to initial conditions
// and boundary conditions.

// RHS is second spatial derivative.  Easy.  From Taylor,
// phi_{i+1,j} = phi_ij + dx * phi,x_ij + dx^2/2 phi,,x_ij + ... // where ,,x means d^2/dx^2
// phi_{i-1,j} = phi_ij - dx * phi,x_ij + dx^2/2 phi,,x_ij + ...
// phi_{i,j+1} = phi_ij + dy * phi,y_ij + dy^2/2 phi,,y_ij + ... // where ,,y means d^2/dy^2
// phi_{i,j-1} = phi_ij - dy * phi,y_ij + dy^2/2 phi,,y_ij + ...
// --------- = -----------------------------------
//           = 4 * phi_ij          + 2 * dx^2/2 phi,,x_ij + 2 * dy^2/2 phi,,y_ij + ...
//           = 4 * phi_ij          + dx^2 phi,,x_ij + dy^2 phi,,y_ij + ...
//           = 4 * phi_ij          + dx^2 (phi,,x_ij + phi,,y_ij) + ... // assume dx=dy
//           = 4 * phi_ij          + dx^2 del^2(phi_ij) + ... // assume dx=dy

// del^2 phi_ij = (-4*phi_{i,j} + phi_{i+1,j} + phi_{i-1,j} phi_{i,j+1} + phi_{i,j-1})/dx^2

// Time derivative gives us many (sigh, many many) options.  Does this
// need to be implicit?  I dunno.  Let's start explicit and see what
// the issues are.

// split time derivative into two steps
// d phi / dt = xi
// d^2 phi / dt^2 = d xi / dt
// d xi / dt = c^2 del^2 phi
// evolve xi and then use xi to evolve phi

// Let's go with forward Euler (omg yuck) for the first version!  Super
// easy!

// xi^{n+1}_i - xi^{n}_i = dt * c^2 * del^2 phi^n_ij
// phi^{n+1}_i - phi^{n}_i = dt * xi^{n+1}_ij


// boundary conditions... reflecting for all boundaries seems fine to start with.
// We can put a forcing function in the middle to start.

};
