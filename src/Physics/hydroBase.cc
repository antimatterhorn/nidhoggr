#include "../State/state.cc"
#include "../Math/vector_math.cc"

namespace Hydro {
template <int dim>
class Hydro {
protected:
    std::vector<FieldListBase*> hydroFieldLists;
    State* state;
public:
    FieldList<double> density;
    FieldList<VectorMath::Vector<dim>> momentum;
    FieldList<double> specific_energy;

    Hydro() {}

    Hydro(int numNodes, State* statePtr) : state(statePtr) {
        density = FieldList<double>("density",numNodes);
        momentum = FieldList<VectorMath::Vector<dim>>("momentum",numNodes);
        specific_energy = FieldList<double>("specific_energy",numNodes);

        state->addFieldList(&density);
        state->addFieldList(&momentum);
        state->addFieldList(&specific_energy);
    }

    virtual ~Hydro() {}

    virtual std::vector<FieldListBase*> getHydroFieldLists() { return hydroFieldLists; }
};
}
