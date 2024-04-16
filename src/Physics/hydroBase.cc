#include "../DataBase/fieldList.hh"
#include "../DataBase/nodeList.hh"
#include "../Math/vector_math.hh"

namespace Hydro {
template <int dim>
class Hydro {
protected:
    NodeList* nodeList;
public:
    FieldList<double> density;
    FieldList<VectorMath::Vector<dim>> momentum;
    FieldList<double> specific_energy;

    Hydro() {}

    Hydro(NodeList* nodeListPtr) : nodeList(nodeListPtr) {
        int numNodes = nodeList->getNumNodes();
        density         = FieldList<double>("density",numNodes);
        momentum        = FieldList<VectorMath::Vector<dim>>("momentum",numNodes);
        specific_energy = FieldList<double>("specific_energy",numNodes);

        nodeList->addFieldList(&density);
        nodeList->addFieldList(&momentum);
        nodeList->addFieldList(&specific_energy);
    }

    virtual ~Hydro() {}
};
}
