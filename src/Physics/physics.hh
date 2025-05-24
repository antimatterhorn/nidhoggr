#ifndef PHYSICS_HH
#define PHYSICS_HH

#include "../Math/vectorMath.hh"
#include "../Type/physicalConstants.hh"
#include "../State/state.hh"
#include "../Boundaries/boundaries.hh"

template <int dim>
class Boundaries; // forward declaration

template <int dim>
class Physics {
protected:
    NodeList* nodeList;
    PhysicalConstants& constants;
    State<dim> state;
    double lastDt;
    std::vector<Boundaries<dim>*> boundaries;
public:
    using Vector = Lin::Vector<dim>;
    using VectorField = Field<Vector>;
    using ScalarField = Field<double>;

    Physics(NodeList* nodeList, PhysicalConstants& constants) : 
        nodeList(nodeList), 
        constants(constants),
        state(nodeList->size()) {
        VerifyFields(nodeList);
    }

    virtual ~Physics() {
        boundaries.clear();
    }

    virtual void
    EnrollScalars(std::initializer_list<const std::string> fields) {
        for (const std::string& name : 
            fields) {
            if (nodeList->getField<double>(name) == nullptr)
                nodeList->insertField<double>(name);
        }
    }

    virtual void
    EnrollVectors(std::initializer_list<const std::string> fields) {
        for (const std::string& name : 
            fields) {
            if (nodeList->getField<Vector>(name) == nullptr)
                nodeList->insertField<Vector>(name);
        }
    }

    virtual void
    ZeroTimeInitialize() {}

    virtual void
    EvaluateDerivatives(const State<dim>* initialState, State<dim>& deriv, const double time, const double dt)  {  }

    virtual void
    PreStepInitialize() {
        State<dim> state = this->state;
        NodeList* nodeList = this->nodeList;

        state.updateFields(nodeList);
        state.updateLastDt(lastDt);
    };

    virtual void
    FinalizeStep(const State<dim>* finalState) {};

    virtual void
    PushState(const State<dim>* stateToPush) {};

    virtual void
    VerifyFields(NodeList* nodeList) {
        int numNodes = nodeList->size();
        if (nodeList->mass() == nullptr)
            nodeList->insertField<double>("mass"); // Add the mass field to the nodeList

        for (const std::string& name : 
            {"position", "velocity"}) {
            if (nodeList->getField<Vector>(name) == nullptr)
                nodeList->insertField<Vector>(name);
        }
    }

    virtual double 
    EstimateTimestep() const { return 0; }

    virtual NodeList*
    getNodeList() const { return nodeList; }

    virtual State<dim>* 
    getState() { return &state; }

    virtual double
    lastStep() const {return lastDt; }

    virtual void
    addBoundary(Boundaries<dim>* boundary){
        boundaries.push_back(boundary);
    }

    virtual void
    ApplyBoundaries() {
        if(boundaries.size() > 0)
            for (const auto& boundary : boundaries)
                boundary->ApplyBoundaries(state,nodeList);
    }
    
    virtual std::string
    name() const { return "Physics"; }

    virtual std::string
    description() const { return "The base class for all physics packages"; }
};

#endif //PHYSICS_HH