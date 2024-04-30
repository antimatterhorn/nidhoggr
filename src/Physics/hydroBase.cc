#include "../DataBase/field.hh"
#include "../DataBase/nodeList.hh"
#include "../Math/vectorMath.hh"

namespace Hydro {
template <int dim>
class Hydro {
protected:
    NodeList* nodeList;
public:
    Field<double> density;
    Field<Lin::Vector<dim>> momentum;
    Field<double> specific_energy;

    Hydro() {}

    Hydro(NodeList* nodeListPtr) : nodeList(nodeListPtr) {
        int numNodes = nodeList->getNumNodes();
        density         = Field<double>("density",numNodes);
        momentum        = Field<Lin::Vector<dim>>("momentum",numNodes);
        specific_energy = Field<double>("specific_energy",numNodes);

        nodeList->addField(&density);
        nodeList->addField(&momentum);
        nodeList->addField(&specific_energy);
    }

    virtual ~Hydro() {}
};
}
