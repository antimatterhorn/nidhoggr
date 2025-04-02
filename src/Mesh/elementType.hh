// elementType.hh
#pragma once

namespace Mesh {
    enum class ElementType {
        Line,        // dim = 1
        Triangle,    // dim = 2
        Quad,
        Tetrahedron, // dim = 3
        Hexahedron
    };
}