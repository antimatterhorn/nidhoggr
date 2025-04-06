#ifndef FEMESH_CC
#define FEMESH_CC

#include "femesh.hh"
#include <stdexcept>
#include <set>
#include "../IO/importObj.cc"

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

template <int dim>
std::vector<std::vector<size_t>> FEMesh<dim>::getElementConnectivity() const {
    std::vector<std::vector<size_t>> connectivity;
    for (const auto& elem : elements) {
        connectivity.push_back(elem->nodeIndices());
    }
    return connectivity;
}

template <int dim>
std::vector<std::pair<ElementType, std::vector<size_t>>>
FEMesh<dim>::getElementInfo() const {
    std::vector<std::pair<ElementType, std::vector<size_t>>> result;
    for (size_t i = 0; i < elements.size(); ++i) {
        result.emplace_back(elementTypes[i], elements[i]->nodeIndices());
    }
    return result;
}


void 
buildFromObj2dHelper(FEMesh<2>& mesh, const std::string& filepath, const std::string& axes) {
    auto [vertices, faces] = importObj2d(filepath, axes);

    for (const auto& v : vertices.getValues()) {
        mesh.addNode(v);
    }

    for (const auto& face : faces.getValues()) {
        if (face.size() == 3) {
            mesh.addElement(ElementType::Triangle, {
                static_cast<size_t>(face[0] - 1),
                static_cast<size_t>(face[1] - 1),
                static_cast<size_t>(face[2] - 1)
            });
        } else if (face.size() == 4) {
            mesh.addElement(ElementType::Quad, {
                static_cast<size_t>(face[0] - 1),
                static_cast<size_t>(face[1] - 1),
                static_cast<size_t>(face[2] - 1),
                static_cast<size_t>(face[3] - 1)
            });
        } else {
            throw std::runtime_error("Only triangle and quad elements are supported in buildFromObj.");
        }
    }
}

template <int dim>
void 
FEMesh<dim>::buildFromObj(const std::string& filepath, const std::string& axes) {
    if constexpr (dim == 2) {
        buildFromObj2dHelper(*this, filepath, axes);
    } else {
        throw std::runtime_error("buildFromObj is only implemented for 2D FEMesh");
    }
}


// Explicit instantiations
template class FEMesh<2>;
template class FEMesh<3>;

} // namespace Mesh

#endif // FEMESH_CC
