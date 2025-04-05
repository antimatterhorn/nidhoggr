#ifndef FEMESH_CC
#define FEMESH_CC

#include "femesh.hh"
#include <stdexcept>
#include <set>

namespace Mesh {

template <int dim>
FEMesh<dim>::FEMesh() : positions("positions") {}

template <int dim>
void 
FEMesh<dim>::addNode(const Vector& position) {
    positions.addValue(position);
}

template <int dim>
void 
FEMesh<dim>::addElement(ElementType type, const std::vector<size_t>& nodeIndices) {
    // Check validity of node indices
    for (size_t idx : nodeIndices) {
        if (idx >= positions.size()) {
            throw std::out_of_range("Node index out of bounds in addElement()");
        }
    }

    // Create element using factory
    auto element = createElement<dim>(type, nodeIndices);
    elements.push_back(element);
    elementTypes.push_back(type);
}

template <int dim>
const std::vector<typename FEMesh<dim>::Vector>& FEMesh<dim>::getNodes() const {
    return positions.getValues();
}

template <int dim>
const std::vector<std::shared_ptr<Element<dim>>>& FEMesh<dim>::getElements() const {
    return elements;
}


template <int dim>
const std::vector<ElementType>& FEMesh<dim>::getElementTypes() const {
    return elementTypes;
}

template <int dim>
void 
FEMesh<dim>::computeNeighbors() {
    neighbors.clear();

    for (const auto& element : elements) {
        const auto& nodeIds = element->nodeIndices();
        for (size_t i = 0; i < nodeIds.size(); ++i) {
            size_t ni = nodeIds[i];
            for (size_t j = 0; j < nodeIds.size(); ++j) {
                if (i != j) {
                    neighbors[ni].push_back(nodeIds[j]);
                }
            }
        }
    }

    // Optional: remove duplicate neighbors
    for (auto& pair : neighbors) {
        std::set<size_t> unique(pair.second.begin(), pair.second.end());
        pair.second.assign(unique.begin(), unique.end());
    }
}

template <int dim>
std::vector<size_t> 
FEMesh<dim>::getNeighbors(size_t nodeId) const {
    auto it = neighbors.find(nodeId);
    if (it != neighbors.end()) {
        return it->second;
    } else {
        return {};
    }
}

template <int dim>
void 
FEMesh<dim>::identifyBoundaryNodes() {
    // Placeholder: could later be based on element adjacency, tags, or external criteria
    boundaryNodes.clear();
    for (size_t i = 0; i < positions.size(); ++i) {
        if (neighbors.find(i) == neighbors.end() || neighbors.at(i).size() < 2) {
            boundaryNodes.push_back(i);
        }
    }
}

template <int dim>
const std::vector<size_t>& FEMesh<dim>::getBoundaryNodes() const {
    return boundaryNodes;
}

// Explicit instantiations
template class FEMesh<2>;
template class FEMesh<3>;

} // namespace Mesh

#endif // FEMESH_CC
