#include "../Field/field.cc"
#include "../Math/vector_math.cc"

namespace Hydro {
template <int dim>
class HydroState {
private:
    std::vector<FieldListBase*> hydroFieldLists;
public:
    FieldList<double> density;
    FieldList<VectorMath::Vector<dim>> momentum;
    FieldList<double> specific_energy;

    HydroState() {}

    HydroState(int numNodes) {
        density = FieldList<double>("density",numNodes);
        momentum = FieldList<VectorMath::Vector<dim>>("momentum",numNodes);
        specific_energy = FieldList<double>("specific_energy",numNodes);

        hydroFieldLists.push_back(&density);
        hydroFieldLists.push_back(&momentum);
        hydroFieldLists.push_back(&specific_energy);
    }

    ~HydroState() {}

    std::vector<FieldListBase*> getHydroFieldLists() { return hydroFieldLists; }
};
}
