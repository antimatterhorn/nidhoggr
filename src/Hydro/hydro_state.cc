#include "../Field/field.cc"
#include "../Math/vector_math.cc"

namespace Hydro {
template <int dim>
class HydroState {
public:
    FieldList<double> density;
    FieldList<VectorMath::Vector<dim>> momentum;
    FieldList<double> specific_energy;

    HydroState() {}

    HydroState(int numNodes) {
        density = FieldList<double>("density",numNodes);
        momentum = FieldList<VectorMath::Vector<dim>>("momentum",numNodes);
        specific_energy = FieldList<double>("specific_energy",numNodes);
    }

    ~HydroState() {}
};
}
