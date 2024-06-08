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

    }

    double
    getCell(int i,int j, std::string fieldName="phi") {
        int idx             = grid->index(j,i,0);
        NodeList* nodeList  = this->nodeList;
        ScalarField* phi    = nodeList->getField<double>(fieldName);
        return phi->getValue(idx);
    }

    virtual void
    FinalizeStep(const State<2>* finalState) override {
        NodeList* nodeList  = this->nodeList;

    }

    virtual double 
    EstimateTimestep() const override { 
        return 1.0;
    }
};