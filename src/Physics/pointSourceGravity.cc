#include "physics.hh"
#include <iostream>

template <int dim>
class PointSourceGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> pointSourceLocation;
    double pointSourceMass;
    double dtmin;
public:
    PointSourceGravity() {}

    PointSourceGravity(NodeList* nodeList, 
                        PhysicalConstants& constants, 
                        Lin::Vector<dim>& pointSourceLocation, 
                        double pointSourceMass) : 
        Physics<dim>(nodeList,constants),
        pointSourceLocation(pointSourceLocation),
        pointSourceMass(pointSourceMass) {

        int numNodes = nodeList->size();
        if (nodeList->getField<Lin::Vector<dim>>("acceleration") == nullptr)
            nodeList->insertField<Lin::Vector<dim>>("acceleration");
        
        Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position");
        std::shared_ptr<Field<Lin::Vector<dim>>> positionSharedPtr(position);
        State<dim>* state = &this->state;        
        state->template addField<Lin::Vector<dim>>(positionSharedPtr);
        Field<Lin::Vector<dim>>* velocity = nodeList->getField<Lin::Vector<dim>>("velocity");
        std::shared_ptr<Field<Lin::Vector<dim>>> velocitySharedPtr(velocity);
        state->template addField<Lin::Vector<dim>>(velocitySharedPtr);
    }

    ~PointSourceGravity() {}

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double t) override {
        //compute accelerations
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* position       = initialState->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* acceleration   = nodeList->getField<Lin::Vector<dim>>("acceleration");
        Field<Lin::Vector<dim>>* velocity       = initialState->template getField<Lin::Vector<dim>>("velocity");

        Field<Lin::Vector<dim>>* dxdt           = deriv.template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* dvdt           = deriv.template getField<Lin::Vector<dim>>("velocity");

        dtmin = 1e30;
        #pragma omp parllel for
        for (int i=0; i<numNodes ; ++i) {
            Lin::Vector<dim> pos = position->getValue(i);
            Lin::Vector<dim> r = (pointSourceLocation - pos);
            Lin::Vector<dim> a = pointSourceMass*constants.G()/(r.mag2())*r.normal();
            acceleration->setValue(i,a);
            double amag = a.mag2();
            double vmag = velocity->getValue(i).mag2();
            //std::cout << "dtmin " << dtmin << " amag " << amag << " vmag " << vmag << std::endl;
            dtmin = std::min(dtmin,vmag/amag);
            //std::cout << "dtmin " << dtmin << std::endl;
            dxdt->setValue(i,velocity->getValue(i)+t*(acceleration->getValue(i)));
            dvdt->setValue(i,acceleration->getValue(i));
        }


    }

    virtual double 
    EstimateTimestep() const override {     
        double timestepCoefficient = 1e-4; // Adjust as needed
        double timestep = timestepCoefficient * sqrt(dtmin);

        return timestep;
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();

        Field<Lin::Vector<dim>>* fposition       = finalState->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* fvelocity       = finalState->template getField<Lin::Vector<dim>>("velocity");

        Field<Lin::Vector<dim>>* position       = nodeList->template getField<Lin::Vector<dim>>("position");
        Field<Lin::Vector<dim>>* velocity       = nodeList->template getField<Lin::Vector<dim>>("velocity");

        position->copyValues(fposition);
        velocity->copyValues(fvelocity);
    }
};
