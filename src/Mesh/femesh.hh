#ifndef FEMESH_HH
#define FEMESH_HH

#pragma once

#include <vector>
#include <string>
#include <unordered_map>
#include <array>

#include "../Math/vectorMath.hh"
#include "../DataBase/field.hh"
#include "../DataBase/nodeList.hh"
#include "elementType.hh"

namespace Mesh {
    template <int dim>
    class FEMesh {
    private:
        std::vector<Lin::Vector<dim>> nodes;
        std::vector<std::vector<size_t>> elements;
        std::vector<ElementType> elementTypes;

        std::unordered_map<size_t, std::vector<size_t>> neighbors;
        std::vector<size_t> boundaryNodes;

    public:
        using Vector = Lin::Vector<dim>;
        using VectorField = Field<Vector>;
        using ScalarField = Field<double>;
        
        FEMesh();

        void addNode(const Vector& position);
        void addElement(ElementType type, const std::vector<size_t>& nodeIndices);

        const std::vector<Vector>& getNodes() const;
        const std::vector<std::vector<size_t>>& getElements() const;
        const std::vector<ElementType>& getElementTypes() const;

        void computeNeighbors();
        void identifyBoundaryNodes();

        std::vector<size_t> getNeighbors(size_t nodeId) const;
        const std::vector<size_t>& getBoundaryNodes() const;
    };
}

#include "femesh.cc"

#endif // FEMESH_HH
