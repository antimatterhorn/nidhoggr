#ifndef VECTORMATH_HH
#define VECTORMATH_HH

#include <vector>
#include <string>
#include <array>
#include <cmath>
#include <algorithm>

namespace Lin {

template <int dim>
class Vector {
public:
    std::array<double, dim> values;

    Vector();
    Vector(std::initializer_list<double> init);
    Vector(std::array<double, dim> init);

    Vector<dim> add(const Vector<dim>& other) const;
    void fill(const double other);
    Vector<dim> sub(const Vector<dim>& other) const;
    double dotProduct(const Vector<dim>& other) const;
    Vector<dim> scalarProduct(double scalar) const;
        // Specialized crossProduct method for Vector<3>
    Vector<3> crossProduct(const Vector<3>& other) const {
        Vector<3> result = {
            values[1] * other.values[2] - values[2] * other.values[1],
            values[2] * other.values[0] - values[0] * other.values[2],
            values[0] * other.values[1] - values[1] * other.values[0]
        };
        return result;
    };

    Vector<dim> operator+(const Vector<dim>& other) const;
    Vector<dim> operator-(const Vector<dim>& other) const;
    Vector<dim> operator*(const double other) const;
    Vector<dim> operator-() const;
    double operator*(const Vector<dim> other) const;
    Vector<dim>& operator=(const Vector<dim>& other);
    Vector<dim>& operator-=(const Vector<dim>& other);
    Vector<dim>& operator+=(const Vector<dim>& other);
    Vector<dim> operator/(const double other) const;
    bool operator==(const Vector<dim> other) const;
    bool operator!=(const Vector<dim> other) const;
    const double& operator[](const unsigned int index) const;
    double& operator[](const unsigned int index);

    double x() const;
    double y() const;
    double z() const;
    void setX(double val);
    void setY(double val);
    void setZ(double val);

    std::string toString() const;
    double mag2() const;
    double magnitude() const;
    Vector<dim> normal() const;

    static Vector<dim> one();
    static Vector<dim> zero();

    static const unsigned numElements = dim;
};

using Vector1D = Vector<1>;
using Vector2D = Vector<2>;
using Vector3D = Vector<3>;

Vector2D quadCentroid(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const Vector2D& p4);
double triangleArea(const Lin::Vector2D& p1, const Lin::Vector2D& p2, const Lin::Vector2D& p3);
double quadArea(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const Vector2D& p4);

template <int dim>
Vector<dim> operator*(const double other, const Vector<dim>& vec);

}

#include "vectorMath.cc"

#endif // VECTORMATH_HH
