#include "physics.hh"
#include "../Mesh/grid.hh"
#include <iostream>
#include <random>

class RockPaperScissors : public Physics<2> {
protected:
    Mesh::Grid<2>* grid;
    double dtmin;
    double A,D;
    std::vector<Field<double>*> xx;

    double 
    nabla(int c, int x, int y) {
        int sx = grid->size_x();
        int sy = grid->size_y();
        double del = 0;
        int idx = grid->index(x,y);
        double thisCon = xx[c]->getValue(idx);
        for(int j=y-1;j<y+2;++j) {
            for(int i=x-1;i<x+2;++i) {
                int ii = ((i%sx)+sx)%sx; // this is like python's mod op
                int jj = ((j%sy)+sy)%sy;
                double fac = 0;
                if(ii!=x && jj!=y)
                    fac = 0.05; 
                else
                    fac = 0.2;
                int iidx = grid->index(ii,jj);
                double thatCon = xx[c]->getValue(iidx);
                del += thatCon*fac; // this is a kernel based approach
            }
        }
        return del;
    }
public:
    using Vector = Lin::Vector<2>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    RockPaperScissors(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<2>* grid, double A, double D) : 
        Physics<2>(nodeList,constants),
        grid(grid), A(A), D(D) {
        if (nodeList->getField<double>("X1") == nullptr)
            nodeList->insertField<double>("X1");
        if (nodeList->getField<double>("X2") == nullptr)
            nodeList->insertField<double>("X2");
        if (nodeList->getField<double>("X3") == nullptr)
            nodeList->insertField<double>("X3");
        if (nodeList->getField<double>("density") == nullptr)
            nodeList->insertField<double>("density");

        grid->assignPositions(nodeList);
        
        ScalarField* X1 = nodeList->getField<double>("X1");
        ScalarField* X2 = nodeList->getField<double>("X2");
        ScalarField* X3 = nodeList->getField<double>("X3");

        xx = {X1,X2,X3};

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, 2);
        for (int j = 0; j < grid->size_y(); ++j) {
            for (int i = 0; i < grid->size_x(); ++i) {
                int choice = dis(gen);
                int idx = grid->index(i,j);
                xx[choice]->setValue(idx,1.0); // or any other initial concentration value for this choice
            }
        }

        State<2>* state = &this->state;
        state->template addField<double>(X1);       
        state->template addField<double>(X2);       
        state->template addField<double>(X3);
    }

    ~RockPaperScissors() {}

    virtual void
    PreStepInitialize() override {
        State<2> state      = this->state;
        NodeList* nodeList  = this->nodeList;
        state.updateFields(nodeList);
    }

    virtual void
    EvaluateDerivatives(const State<2>* initialState, State<2>& deriv, const double time, const double dt) override {  
        NodeList* nodeList  = this->nodeList;
        int numNodes        = nodeList->size();

        ScalarField* rho = nodeList->getField<double>("density");

        ScalarField* X1 = initialState->template getField<double>("X1");
        ScalarField* X2 = initialState->template getField<double>("X2");
        ScalarField* X3 = initialState->template getField<double>("X3");
        
        xx = {X1,X2,X3};

        ScalarField* Dx1Dt = deriv.template getField<double>("X1");
        ScalarField* Dx2Dt = deriv.template getField<double>("X2");
        ScalarField* Dx3Dt = deriv.template getField<double>("X3");

        std::vector<ScalarField*> DxxDt = {Dx1Dt,Dx2Dt,Dx3Dt};

        int sx = grid->size_x();
        int sy = grid->size_y();

        #pragma omp parallel for collapse(2)
        for (int j = 0; j < sy; ++j) {
            for (int i = 0; i < sx; ++i) {
                int idx = grid->index(i,j);
                rho->setValue(idx,0);
                for(int c=0;c<3;++c) 
                    rho->setValue(idx, rho->getValue(idx) + xx[c]->getValue(idx));
            }
        }

        #pragma omp parallel for collapse(2)
        for(int c=0;c<3;++c) {
            for (int j = 0; j < sy; ++j) {
                for (int i = 0; i < sx; ++i) {
                    int idx = grid->index(i,j);
                    int nc = (c+1)%3;   // this only works for 3 concentrations!
                    double r = rho->getValue(idx);
                    double u = xx[c]->getValue(idx);
                    double dudt = D*nabla(c,i,j);
                    dudt += u*(1.0-r);
                    dudt -= u*A*xx[c]->getValue(idx);
                    DxxDt[c]->setValue(idx,dudt);
                }
            }
        }

    }

    std::array<double, 3>
    getCell(int i, int j, std::string fieldName = "doesntmatter") {
        int idx = grid->index(i,j);
        return {xx[0]->getValue(idx),xx[1]->getValue(idx),xx[2]->getValue(idx)};
    }

    virtual void
    FinalizeStep(const State<2>* finalState) override {
        NodeList* nodeList  = this->nodeList;

        ScalarField* fX1 = finalState->template getField<double>("X1");
        ScalarField* fX2 = finalState->template getField<double>("X2");
        ScalarField* fX3 = finalState->template getField<double>("X3");

        ScalarField* X1 = nodeList->getField<double>("X1");
        ScalarField* X2 = nodeList->getField<double>("X2");
        ScalarField* X3 = nodeList->getField<double>("X3");

        X1->copyValues(fX1);
        X2->copyValues(fX2);
        X3->copyValues(fX3);

        xx = {X1,X2,X3};
        for (int i=0; i<nodeList->getNumNodes(); ++i) {
            for (int c=0;c<3;++c) {
                double X = xx[c]->getValue(i);
                X = std::max(std::min(1.0,X),0.0);
                xx[c]->setValue(i,X);
            }
        }

    }

    virtual double 
    EstimateTimestep() const override { 
        return 1.0;
    }
};