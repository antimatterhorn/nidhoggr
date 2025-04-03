#pragma once

#include <memory>
#include <stdexcept>
#include "element.hh"

namespace Mesh {

template <int dim>
std::shared_ptr<Element<dim>> createElement(ElementType type, const std::vector<size_t>& nodeIndices) {
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
