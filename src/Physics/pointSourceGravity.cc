#include "physics.hh"
#include <iostream>

template <int dim>
class PointSourceGravity : public Physics<dim> {
protected:
    Lin::Vector<dim> pointSourceLocation;
    Lin::Vector<dim> pointSourceVelocity;
    double pointSourceMass;
    double dtmin;
    double lastUpdateTime = -1.0; // Tracks last update time
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    PointSourceGravity(NodeList* nodeList,
                        PhysicalConstants& constants,
                        Vector& pointSourceLocation,
                        Vector& pointSourceVelocity,
                        double pointSourceMass) :
        Physics<dim>(nodeList,constants),
        pointSourceLocation(pointSourceLocation),
        pointSourceVelocity(pointSourceVelocity),
        pointSourceMass(pointSourceMass) {
        

        int numNodes = nodeList->size();
        for (const std::string& name : 
            {"acceleration", "position", "velocity"}) {
            if (nodeList->getField<Vector>(name) == nullptr)
                nodeList->insertField<Vector>(name);
        }

        State<dim>* state = &this->state;
        VectorField* position = nodeList->getField<Vector>("position");
        state->template addField<Vector>(position);
        VectorField* velocity = nodeList->getField<Vector>("velocity");
        state->template addField<Vector>(velocity);
        std::cout << "point source mass: "<< pointSourceMass<<"\n";
    }

    // PointSourceGravity(NodeList* nodeList,
    //         PhysicalConstants& constants,
    //         Vector& pointSourceLocation,
    //         double pointSourceMass) :
    //     PointSourceGravity(nodeList, constants, pointSourceLocation, Vector::zero(), pointSourceMass) {}
    // This can't work since it has to be a pointer to a vector and nobody is holding it!

    
    ~PointSourceGravity() {}

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt) override {
        // Update point source position once per timestep
        if (time != lastUpdateTime) {
            pointSourceLocation += pointSourceVelocity * dt;
            lastUpdateTime = time;
        }
        
        //compute accelerations
        NodeList* nodeList = this->nodeList;
        PhysicalConstants constants = this->constants;
        int numNodes = nodeList->size();

        VectorField* position       = initialState->template getField<Vector>("position");
        VectorField* acceleration   = nodeList->getField<Vector>("acceleration");
        // ^ this field is just for reference and isn't actually used to calculate anything
        VectorField* velocity       = initialState->template getField<Vector>("velocity");

        VectorField* dxdt           = deriv.template getField<Vector>("position");
        VectorField* dvdt           = deriv.template getField<Vector>("velocity");

        double local_dtmin = 1e30;

        #pragma omp parallel for reduction(min:local_dtmin)
        for (int i=0; i<numNodes ; ++i) {
            Vector pos = position->getValue(i);
            Vector r = (pointSourceLocation - pos);
            Vector a = pointSourceMass*constants.G()/(r.mag2())*r.normal();
            Vector v = velocity->getValue(i);
            acceleration->setValue(i,a);
            double amag = a.mag2();
            double vmag = v.mag2();
            local_dtmin = std::min(local_dtmin,vmag/amag);
            dxdt->setValue(i,v+dt*a);
            dvdt->setValue(i,a);
        }

        dtmin = local_dtmin;
        this->lastDt = dt;
    }

    virtual double
    EstimateTimestep() const override {
        double timestepCoefficient = 1e-4; // Adjust as needed
        double timestep = timestepCoefficient * sqrt(dtmin);

        return timestep;
    }

    virtual void
    FinalizeStep(const State<dim>* finalState) override {
        PushState(finalState);
    }

    virtual void
    PushState(const State<dim>* stateToPush) override {
        NodeList* nodeList = this->nodeList;
        int numNodes = nodeList->size();
        State<dim> state = this->state;

        VectorField* position       = nodeList->template getField<Vector>("position");
        VectorField* velocity       = nodeList->template getField<Vector>("velocity");

        VectorField* fposition       = stateToPush->template getField<Vector>("position");
        VectorField* fvelocity       = stateToPush->template getField<Vector>("velocity");

        position->copyValues(fposition);
        velocity->copyValues(fvelocity);

        if (stateToPush != &(state))
        {
            VectorField* sposition       = state.template getField<Vector>("position");
            VectorField* svelocity       = state.template getField<Vector>("velocity");
            sposition->copyValues(fposition);
            svelocity->copyValues(fvelocity);
        }
    }

    virtual std::string name() const override { return "pointSourceGravity"; }
    virtual std::string description() const override {
        return "Point source gravity physics package for particles"; }
};
