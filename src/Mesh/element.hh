#pragma once

#include <vector>
#include "../Math/vectorMath.hh"
#include "elementType.hh"
#include <stdexcept>

namespace Mesh {
    template <int dim>
    class Element {
    public:
        using Vector = Lin::Vector<dim>;

        Element(ElementType type, const std::vector<size_t>& nodeIndices)
            : mType(type), mNodeIndices(nodeIndices) {}

        virtual ~Element() = default;

        virtual double computeMeasure(const std::vector<Vector>& positions) const = 0;

        ElementType type() const { return mType; }
        const std::vector<size_t>& nodeIndices() const { return mNodeIndices; }

    protected:
        ElementType mType;
        std::vector<size_t> mNodeIndices;
    };

    class TriangleElement : public Element<2> {
    public:
        using Vector = Lin::Vector<2>;
    
        TriangleElement(const std::vector<size_t>& nodeIndices)
            : Element<2>(ElementType::Triangle, nodeIndices) {}
    
        double computeMeasure(const std::vector<Vector>& positions) const override {
            const auto& A = positions[mNodeIndices[0]];
            const auto& B = positions[mNodeIndices[1]];
            const auto& C = positions[mNodeIndices[2]];
            return Lin::triangleArea(A, B, C);  
        }
    };

    class QuadElement : public Element<2> {
    public:
        using Vector = Lin::Vector<2>;
    
        QuadElement(const std::vector<size_t>& nodeIndices)
            : Element<2>(ElementType::Quad, nodeIndices) {
            if (nodeIndices.size() != 4) {
                throw std::invalid_argument("QuadElement must have exactly 4 nodes");
            }
        }
    
        double computeMeasure(const std::vector<Vector>& positions) const override {
            const auto& A = positions[mNodeIndices[0]];
            const auto& B = positions[mNodeIndices[1]];
            const auto& C = positions[mNodeIndices[2]];
            const auto& D = positions[mNodeIndices[3]];
            return Lin::quadArea(A, B, C, D);
        }
    };
        
    template <int dim>
    std::shared_ptr<Element<dim>> 
    createElement(ElementType type, const std::vector<size_t>& nodeIndices) {
        if constexpr (dim == 2) {
            switch (type) {
                case ElementType::Triangle:
                    return std::make_shared<TriangleElement>(nodeIndices);
                case ElementType::Quad:
                    return std::make_shared<QuadElement>(nodeIndices);
                default:
                    throw std::invalid_argument("Unsupported ElementType for 2D mesh");
            }
        } else if constexpr (dim == 3) {
            // add 3D support here later
            throw std::invalid_argument("3D elements not implemented yet");
        } else {
            throw std::invalid_argument("Unsupported spatial dimension");
        }
    }    
}
