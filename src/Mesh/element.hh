#pragma once

#include <vector>
#include "../Math/vectorMath.hh"
#include "elementType.hh"
#include <stdexcept>
#include <Eigen/Dense>

namespace Mesh {
    template <int dim>
    class Element {
    public:
        using Vector = Lin::Vector<dim>;

        Element(ElementType type, const std::vector<size_t>& nodeIndices)
            : mType(type), mNodeIndices(nodeIndices) {}

        virtual ~Element() = default;

        virtual double computeArea(const std::vector<Vector>& positions) const = 0;
        virtual Vector computeCentroid(const std::vector<Vector>& positions) const = 0;
        virtual Eigen::MatrixXd computeStiffnessMatrix(const std::vector<Vector>& positions) const = 0;

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
    
        double 
        computeArea(const std::vector<Vector>& positions) const override {
            const auto& A = positions[mNodeIndices[0]];
            const auto& B = positions[mNodeIndices[1]];
            const auto& C = positions[mNodeIndices[2]];
            return Lin::triangleArea(A, B, C);  
        }

        Vector 
        computeCentroid(const std::vector<Vector>& positions) const override {
            const auto& A = positions[mNodeIndices[0]];
            const auto& B = positions[mNodeIndices[1]];
            const auto& C = positions[mNodeIndices[2]];
            return Lin::triangleCentroid(A, B, C);
        }

        Eigen::MatrixXd 
        computeStiffnessMatrix(const std::vector<Vector>& positions) const override {
            const auto& A = positions[mNodeIndices[0]];
            const auto& B = positions[mNodeIndices[1]];
            const auto& C = positions[mNodeIndices[2]];
        
            // Build coordinate matrix
            double x1 = A.x(), y1 = A.y();
            double x2 = B.x(), y2 = B.y();
            double x3 = C.x(), y3 = C.y();
        
            // Compute area
            double area = Lin::triangleArea(A, B, C);
            if (area <= 0.0) throw std::runtime_error("Degenerate triangle in stiffness matrix computation");
        
            // Shape function gradients
            Eigen::Matrix<double, 2, 3> Bmat;  // 2 rows (∂/∂x and ∂/∂y), 3 shape functions
        
            Bmat(0, 0) = y2 - y3;
            Bmat(0, 1) = y3 - y1;
            Bmat(0, 2) = y1 - y2;
        
            Bmat(1, 0) = x3 - x2;
            Bmat(1, 1) = x1 - x3;
            Bmat(1, 2) = x2 - x1;
        
            Bmat /= (2.0 * area);
        
            // Simple isotropic constant conductivity k = 1
            double k = 1.0;
        
            // Compute K = k * A * (B^T * B)
            Eigen::MatrixXd Ke = k * area * Bmat.transpose() * Bmat;
        
            return Ke;  // 3x3 matrix
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
    
        double 
        computeArea(const std::vector<Vector>& positions) const override {
            const auto& A = positions[mNodeIndices[0]];
            const auto& B = positions[mNodeIndices[1]];
            const auto& C = positions[mNodeIndices[2]];
            const auto& D = positions[mNodeIndices[3]];
            return Lin::quadArea(A, B, C, D);
        }

        Vector 
        computeCentroid(const std::vector<Vector>& positions) const override {
            const auto& A = positions[mNodeIndices[0]];
            const auto& B = positions[mNodeIndices[1]];
            const auto& C = positions[mNodeIndices[2]];
            const auto& D = positions[mNodeIndices[3]];
            return Lin::quadCentroid(A, B, C, D);
        }

        Eigen::MatrixXd 
        computeStiffnessMatrix(const std::vector<Vector>& positions) const override {
            assert(mNodeIndices.size() == 4 && "QuadElement must have 4 nodes");
        
            const auto& A = positions[mNodeIndices[0]];
            const auto& B = positions[mNodeIndices[1]];
            const auto& C = positions[mNodeIndices[2]];
            const auto& D = positions[mNodeIndices[3]];
        
            std::array<Vector, 4> nodeCoords = {A, B, C, D};
        
            // 2x2 Gauss points and weights for reference square [-1, 1] x [-1, 1]
            constexpr double g = 0.5773502691896257;  // 1 / sqrt(3)
            std::array<std::pair<double, double>, 4> gaussPoints = {{
                {-g, -g}, { g, -g}, { g,  g}, { -g,  g}
            }};
            std::array<double, 4> weights = {1.0, 1.0, 1.0, 1.0};
        
            Eigen::MatrixXd Ke = Eigen::MatrixXd::Zero(4, 4);
        
            for (int gp = 0; gp < 4; ++gp) {
                double xi = gaussPoints[gp].first;
                double eta = gaussPoints[gp].second;
        
                // Shape function derivatives w.r.t. reference coords (ξ, η)
                Eigen::Matrix<double, 4, 2> dNdXi;
                dNdXi(0, 0) = -0.25 * (1 - eta); dNdXi(0, 1) = -0.25 * (1 - xi);
                dNdXi(1, 0) =  0.25 * (1 - eta); dNdXi(1, 1) = -0.25 * (1 + xi);
                dNdXi(2, 0) =  0.25 * (1 + eta); dNdXi(2, 1) =  0.25 * (1 + xi);
                dNdXi(3, 0) = -0.25 * (1 + eta); dNdXi(3, 1) =  0.25 * (1 - xi);
        
                // Compute Jacobian matrix J
                Eigen::Matrix2d J = Eigen::Matrix2d::Zero();
                for (int i = 0; i < 4; ++i) {
                    J(0, 0) += dNdXi(i, 0) * nodeCoords[i].x();
                    J(0, 1) += dNdXi(i, 0) * nodeCoords[i].y();
                    J(1, 0) += dNdXi(i, 1) * nodeCoords[i].x();
                    J(1, 1) += dNdXi(i, 1) * nodeCoords[i].y();
                }
        
                double detJ = J.determinant();
                if (detJ <= 0.0) throw std::runtime_error("Invalid quad element: Jacobian determinant <= 0");
        
                Eigen::Matrix2d invJ = J.inverse();
        
                // ∇N_i in physical coordinates: dN/dx = dN/dXi * inv(J)
                Eigen::Matrix<double, 4, 2> dNdX;
                for (int i = 0; i < 4; ++i) {
                    Eigen::Vector2d gradXi = dNdXi.row(i);
                    Eigen::Vector2d gradX = invJ * gradXi;
                    dNdX(i, 0) = gradX(0);
                    dNdX(i, 1) = gradX(1);
                }
        
                // Compute local stiffness contribution at this Gauss point
                Eigen::MatrixXd Kgp = Eigen::MatrixXd::Zero(4, 4);
                for (int i = 0; i < 4; ++i) {
                    for (int j = 0; j < 4; ++j) {
                        double dot = dNdX(i, 0) * dNdX(j, 0) + dNdX(i, 1) * dNdX(j, 1);
                        Kgp(i, j) = dot;
                    }
                }
        
                Ke += Kgp * detJ * weights[gp];
            }
        
            return Ke;
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
