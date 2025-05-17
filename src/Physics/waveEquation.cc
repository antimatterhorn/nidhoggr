#include "physics.hh"
#include "../Mesh/grid.hh"
#include "../IO/importDepthMap.hh"
#include <iostream>

template <int dim>
class WaveEquation : public Physics<dim> {
protected:
    Mesh::Grid<dim>* grid;
    Mesh::Grid<2>* grid2d;
    bool ocean = false;
    double C;
    double dtmin;
    double dxmin = 1e30;
    std::vector<int> insideIds;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    WaveEquation(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<dim>* grid, double C) : 
        Physics<dim>(nodeList,constants),
        grid(grid), C(C) {
        VerifyWaveFields();

        grid->assignPositions(nodeList);

        ScalarField* cs = nodeList->getField<double>("soundSpeed");
        for (int i=0; i<nodeList->getNumNodes();++i) cs->setValue(i,C);
        for (int i = 0; i < dim; ++i)
            dxmin = std::min(dxmin, grid->spacing(i));
    }

    WaveEquation(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<2>* grid, const std::string& depthMap) : 
        Physics<dim>(nodeList, constants),
        grid2d(grid), C(constants.ESurfaceGrav()), ocean(true) {
        if (dim != 2) {
            std::cerr << "Error: This constructor can only be used with dim = 2" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        VerifyWaveFields();

        grid2d->assignPositions(nodeList);

        if (nodeList->getField<double>("depth") == nullptr)
            nodeList->insertField<double>("depth");

        grid2d->template insertField<double>("depth");
        ImportDepthMap map(depthMap);
        map.populateDepthField(grid2d);

        ScalarField* depth      = grid2d->template getField<double>("depth");
        ScalarField* nodeDepth  = nodeList->getField<double>("depth");
        nodeDepth->copyValues(depth);

        ScalarField* cs = nodeList->getField<double>("soundSpeed");
        double maxC = 0;
        #pragma omp parallel for reduction(max:maxC)
        for (int i=0; i<nodeList->getNumNodes();++i) {
            double c = (depth->getValue(i) < 0 ? sqrt(C*std::abs(depth->getValue(i))) : 0);
            cs->setValue(i,c);
            maxC = std::max(c,maxC);
        }
        C = maxC;

        for (int i = 0; i < dim; ++i)
            dxmin = std::min(dxmin, grid2d->spacing(i));
    }

    ~WaveEquation() {}

    virtual void
    ZeroTimeInitialize() override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        for (int i=0; i<numNodes; ++i) {
            if (ocean) {
                if (!grid2d->onBoundary(i))
                    insideIds.push_back(i);
            }
            else {
                if (!grid->onBoundary(i))
                    insideIds.push_back(i);
            }
        }
    }

    void
    VerifyWaveFields() {
        NodeList* nodeList = this->nodeList;
        if (nodeList->getField<double>("phi") == nullptr)
            nodeList->insertField<double>("phi");
        if (nodeList->getField<double>("xi") == nullptr)
            nodeList->insertField<double>("xi");
        if (nodeList->getField<double>("maxphi") == nullptr)
            nodeList->insertField<double>("maxphi");
        if (nodeList->getField<double>("soundSpeed") == nullptr)
            nodeList->insertField<double>("soundSpeed");       

        ScalarField* xi     = nodeList->getField<double>("xi");
        ScalarField* phi    = nodeList->getField<double>("phi");

        State<dim>* state = &this->state;
        state->template addField<double>(xi);
        state->template addField<double>(phi);
    }

    virtual void
    PreStepInitialize() override {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;
        state.updateFields(nodeList);
    }

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        
        ScalarField* xi     = initialState->template getField<double>("xi");
        ScalarField* phi    = initialState->template getField<double>("phi");

        ScalarField* DxiDt  = deriv.template getField<double>("xi");
        ScalarField* DphiDt = deriv.template getField<double>("phi");

        ScalarField* cs     = nodeList->getField<double>("soundSpeed");

        double local_dtmin = 1e30;

        #pragma omp parallel for reduction(min:local_dtmin)
        for (int p = 0; p < insideIds.size(); ++p) {
            int i = insideIds[p];
        
            double c = cs->getValue(i);
            double phi_i = phi->getValue(i);
            double laplace = 0.0;
        
            if constexpr (dim == 1) {
                double dx = grid->dx;
                int left = i - 1;
                int right = i + 1;
                if (left < 0) left = i;
                if (right >= grid->nx) right = i;
        
                laplace = (phi->getValue(right) - 2 * phi_i + phi->getValue(left)) / (dx * dx);
            }
        
            if constexpr (dim == 2) {
                auto [ix, iy, _] = grid->indexToCoordinates(i);
                double dx = grid->dx;
                double dy = grid->dy;
                int nx = grid->getnx();
                int ny = grid->getny();
            
                auto get = [&](int x, int y) -> double {
                    if (x < 0 || x >= nx || y < 0 || y >= ny) return phi_i;
                    return phi->getValue(grid->index(x, y));
                };
            
                laplace = (get(ix+1, iy) - 2*phi_i + get(ix-1, iy)) / (dx * dx) +
                          (get(ix, iy+1) - 2*phi_i + get(ix, iy-1)) / (dy * dy);
            }
            
        
            if constexpr (dim == 3) {
                auto [ix, iy, iz] = grid->indexToCoordinates(i);
                double dx = grid->dx, dy = grid->dy, dz = grid->dz;
                int nx = grid->getnx(), ny = grid->getny(), nz = grid->getnz();
            
                auto get = [&](int x, int y, int z) -> double {
                    if (x < 0 || x >= nx || y < 0 || y >= ny || z < 0 || z >= nz) return phi_i;
                    return phi->getValue(grid->index(x, y, z));
                };
            
                laplace = (get(ix+1, iy, iz) - 2*phi_i + get(ix-1, iy, iz)) / (dx * dx) +
                          (get(ix, iy+1, iz) - 2*phi_i + get(ix, iy-1, iz)) / (dy * dy) +
                          (get(ix, iy, iz+1) - 2*phi_i + get(ix, iy, iz-1)) / (dz * dz);
            }
            
        
            DxiDt->setValue(i, laplace * c * c);
            DphiDt->setValue(i, dt * DxiDt->getValue(i) + xi->getValue(i));

            local_dtmin = std::min(local_dtmin, 0.2 * dxmin / c);
        }
        dtmin = local_dtmin;
    }

    double
    getCell(int i,int j, std::string fieldName="phi") {
        int idx = (ocean ? grid2d->index(i,j,0) : grid->index(i,j,0));
        NodeList* nodeList  = this->nodeList;
        ScalarField* phi    = nodeList->getField<double>(fieldName);
        return phi->getValue(idx);
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        ScalarField* fxi    = finalState->template getField<double>("xi");
        ScalarField* fphi   = finalState->template getField<double>("phi");

        ScalarField* xi     = nodeList->getField<double>("xi");
        ScalarField* phi    = nodeList->getField<double>("phi");
        ScalarField* mphi   = nodeList->getField<double>("maxphi");

        xi->copyValues(fxi);
        phi->copyValues(fphi);

        for (int i=0; i<numNodes; ++i)
            mphi->setValue(i,std::max(mphi->getValue(i),std::abs(phi->getValue(i))));
    }

    virtual double 
    EstimateTimestep() const override { 
        return dtmin;
    }

    virtual std::string name() const override { return "waveEquation"; }
    virtual std::string description() const override {
        return "Acoustic wave physics package for grids"; }
};


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
