#include "physics.hh"
#include "../Mesh/grid.hh"
#include "../IO/importDepthMap.hh"
#include <iostream>

class WaveHeight : public Physics<2> {
protected:
    Mesh::Grid<2>* grid;
    double dtmin;
    double g;
public:
    using Vector = Lin::Vector<2>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    WaveHeight(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<2>* grid, std::string& depthMap) : 
        Physics<2>(nodeList,constants),
        grid(grid), g(constants.ESurfaceGrav()) {
        if (nodeList->getField<double>("phi") == nullptr)
            nodeList->insertField<double>("phi");
        if (nodeList->getField<double>("xi") == nullptr)
            nodeList->insertField<double>("xi");
        if (nodeList->getField<double>("depth") == nullptr)
            nodeList->insertField<double>("depth");
        /* 
        This sets the nodeList positions field to whatever is inside Grid positions. This should ideally
        happen with any physics package that uses a mesh, so this is a bit clunky to have here.
        */
        grid->assignPositions(nodeList);
        
        State<2>* state = &this->state;
        ScalarField* xi = nodeList->getField<double>("xi");
        state->template addField<double>(xi);
        ScalarField* phi = nodeList->getField<double>("phi");
        state->template addField<double>(phi);

        /* 
        Now the grid should hold the depth Field, not the nodeList, and the depth map should be passed
        into this field. 
        */
        grid->insertField<double>("depth");
        ImportDepthMap map(depthMap);
        map.populateDepthField(grid);

        ScalarField* depth = grid->template getField<double>("depth");
        ScalarField* nodeDepth = nodeList->getField<double>("depth");
        nodeDepth->copyValues(depth);
    }

    ~WaveHeight() {}

    virtual void
    PreStepInitialize() override {
        State<2> state = this->state;
        NodeList* nodeList = this->nodeList;
        state.updateFields(nodeList);

        ScalarField* xi = nodeList->getField<double>("xi");
        ScalarField* phi = nodeList->getField<double>("phi");

        ScalarField* sxi = state.getField<double>("xi");
        ScalarField* sphi = state.getField<double>("phi");

        for (int i=0; i<nodeList->size(); ++i) {
            double xii = xi->getValue(i);
            double phii = phi->getValue(i);
            double sxii = sxi->getValue(i);
            double sphii = sphi->getValue(i);
            if (i==210) {
                printf("in prestep %d %f %f %f %f\n",i,phii,xii,sphii,sxii);
            }
        }
    }

    virtual void
    EvaluateDerivatives(const State<2>* initialState, State<2>& deriv, const double time, const double dt) override {  
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        
        ScalarField* xi     = initialState->template getField<double>("xi");
        ScalarField* phi    = initialState->template getField<double>("phi");

        ScalarField* DxiDt  = deriv.template getField<double>("xi");
        ScalarField* DphiDt = deriv.template getField<double>("phi");

        ScalarField* h      = grid->template getField<double>("depth");
        
        #pragma omp parallel for
        for (int i = 0; i < grid->size(); ++i) {
            std::vector<int> neighbors = grid->getNeighboringCells(i);
            double dx = grid->getdx();
            double dy = grid->getdy();

            double nablaPhi = 0.0;
            double nablaH   = 0.0;
            double phii     = phi->getValue(i);
            double hi       = h->getValue(i);
            double xii      = xi->getValue(i);

            int rightNeighbor = neighbors[0];
            int leftNeighbor = neighbors[1];
            double firstDerivativeX = (phi->getValue(rightNeighbor) - phi->getValue(leftNeighbor)) / (2 * dx);
            nablaPhi += firstDerivativeX * firstDerivativeX;
            if (i==210)
            {
                printf("in eval %f, %03d %d %d: phii=%04.4f phiL=%f phiR=%f nablaPhi=%f\n",g,i,leftNeighbor,rightNeighbor,phii,phi->getValue(leftNeighbor),phi->getValue(rightNeighbor),nablaPhi);
            }
            firstDerivativeX = (h->getValue(rightNeighbor) - h->getValue(leftNeighbor)) / (2 * dx);
            nablaH += firstDerivativeX * firstDerivativeX;

            int downNeighbor = neighbors[2];
            int upNeighbor = neighbors[3];
            double firstDerivativeY = (phi->getValue(upNeighbor) - phi->getValue(downNeighbor)) / (2 * dy);
            nablaPhi += firstDerivativeY * firstDerivativeY;
            firstDerivativeY = (h->getValue(upNeighbor) - h->getValue(downNeighbor)) / (2 * dy);
            nablaH += firstDerivativeY * firstDerivativeY;

            nablaPhi = sqrt(nablaPhi);
            nablaH   = sqrt(nablaH);

            double dxidti = g*(phii*nablaH + hi*nablaPhi);
            double dphidti = dt*dxidti + xii;

            DxiDt->setValue(i,dxidti); 
            DphiDt->setValue(i,dphidti); 
        }
    }

    double
    getCell(int i,int j, std::string fieldName="phi") {
        int idx = grid->index(j,i,0);
        NodeList* nodeList = this->nodeList;
        ScalarField* phi = nodeList->getField<double>(fieldName);
        return phi->getValue(idx);
    }

    virtual void
    FinalizeStep(const State<2>* finalState) override {
        NodeList* nodeList = this->nodeList;

        ScalarField* fxi = finalState->template getField<double>("xi");
        ScalarField* fphi = finalState->template getField<double>("phi");

        ScalarField* xi = nodeList->getField<double>("xi");
        ScalarField* phi = nodeList->getField<double>("phi");

        xi->copyValues(fxi);
        phi->copyValues(fphi);

        for (int i=0; i<nodeList->size(); ++i) {
            double xii = xi->getValue(i);
            double phii = phi->getValue(i);
            double fxii = fxi->getValue(i);
            double fphii = fphi->getValue(i);
            if (i==210) {
                printf("in final %d %f %f %f %f\n",i,phii,xii,fphii,fxii);
            }
        }
    }

    virtual double 
    EstimateTimestep() const override { 
        return 0;
    }


// d^2 phi / dt^2 = g*del (h*phi)
//                = g*(phi*del h + h*del phi)
// xi = d phi / dt

};
