#ifndef VECTORMATH_CC
#define VECTORMATH_CC

#include "vectorMath.hh"

namespace Lin {

template <int dim>
Vector<dim>::Vector() {
    for (int i = 0; i < dim; ++i) {
        values[i] = 0.0;
    }
}

template <int dim>
Vector<dim>::Vector(std::initializer_list<double> init) {
    if (init.size() != dim) {
        throw std::invalid_argument("Invalid number of arguments for Vector constructor");
    }
    std::copy(init.begin(), init.end(), values.begin());
}

template <int dim>
Vector<dim>::Vector(std::array<double, dim> init) {
    values = init;
}

template <int dim>
Vector<dim> Vector<dim>::add(const Vector<dim>& other) const {
    Vector<dim> result;
    for (int i = 0; i < dim; ++i) {
        result.values[i] = values[i] + other.values[i];
    }
    return result;
}

template <int dim>
void Vector<dim>::fill(const double other) {
    for (int i = 0; i < dim; ++i) {
        values[i] = other;
    }
}

template <int dim>
Vector<dim> Vector<dim>::sub(const Vector<dim>& other) const {
    Vector<dim> result;
    for (int i = 0; i < dim; ++i) {
        result.values[i] = values[i] - other.values[i];
    }
    return result;
}

template <int dim>
double Vector<dim>::dotProduct(const Vector<dim>& other) const {
    double result = 0.0;
    for (int i = 0; i < dim; ++i) {
        result += values[i] * other.values[i];
    }
    return result;
}

template <int dim>
Vector<dim> Vector<dim>::scalarProduct(double scalar) const {
    Vector<dim> result;
    for (int i = 0; i < dim; ++i) {
        result.values[i] = values[i] * scalar;
    }
    return result;
}

template <int dim>
Vector<dim> Vector<dim>::operator+(const Vector<dim>& other) const {
    return add(other);
}

template <int dim>
Vector<dim> Vector<dim>::operator-(const Vector<dim>& other) const {
    return sub(other);
}

template <int dim>
Vector<dim> Vector<dim>::operator*(const double other) const {
    return scalarProduct(other);
}

template <int dim>
Vector<dim> Vector<dim>::operator-() const {
    return scalarProduct(-1.0);
}

template <int dim>
double Vector<dim>::operator*(const Vector<dim> other) const {
    return dotProduct(other);
}

template <int dim>
Vector<dim>& Vector<dim>::operator=(const Vector<dim>& other) {
    if (this != &other) {
        for (int i = 0; i < dim; ++i) {
            values[i] = other.values[i];
        }
    }
    return *this;
}

template <int dim>
Vector<dim>& Vector<dim>::operator-=(const Vector<dim>& other) {
    for (int i = 0; i < dim; ++i) {
        values[i] -= other.values[i];
    }
    return *this;
}

template <int dim>
Vector<dim>& Vector<dim>::operator+=(const Vector<dim>& other) {
    for (int i = 0; i < dim; ++i) {
        values[i] += other.values[i];
    }
    return *this;
}

template <int dim>
Vector<dim> Vector<dim>::operator/(const double other) const {
    return scalarProduct(1.0 / other);
}

template <int dim>
bool Vector<dim>::operator==(const Vector<dim> other) const {
    for (int i = 0; i < dim; ++i) {
        if (values[i] != other.values[i]) {
            return false;
        }
    }
    return true;
}

template <int dim>
bool Vector<dim>::operator!=(const Vector<dim> other) const {
    return !(*this == other);
}

template <int dim>
const double& Vector<dim>::operator[](const unsigned int index) const {
    return values[index];
}

template <int dim>
double& Vector<dim>::operator[](const unsigned int index) {
    return values[index];
}

template <int dim>
double Vector<dim>::x() const {
    return values.size() > 0 ? values[0] : 0.0;
}

template <int dim>
double Vector<dim>::y() const {
    return values.size() > 1 ? values[1] : 0.0;
}

template <int dim>
double Vector<dim>::z() const {
    return values.size() > 2 ? values[2] : 0.0;
}

template <int dim>
void Vector<dim>::setX(double val) {
    values[0] = val;
}

template <int dim>
void Vector<dim>::setY(double val) {
    if (values.size() > 1) {
        values[1] = val;
    }
}

template <int dim>
void Vector<dim>::setZ(double val) {
    if (values.size() > 2) {
        values[2] = val;
    }
}

template <int dim>
std::string Vector<dim>::toString() const {
    std::string result = "(";
    for (int i = 0; i < dim; ++i) {
        if (i > 0) {
            result += ", ";
        }
        result += std::to_string(values[i]);
    }
    result += ")";
    return result;
}

template <int dim>
double Vector<dim>::mag2() const {
    double result = 0.0;
    for (double value : values) {
        result += value * value;
    }
    return result;
}

template <int dim>
double Vector<dim>::magnitude() const {
    return std::sqrt(mag2());
}

template <int dim>
Vector<dim> Vector<dim>::normal() const {
    Vector<dim> result;
    double mag = this->magnitude();
    for (int i = 0; i < dim; ++i) {
        result[i] = values[i] / mag;
    }
    return result;
}

template <int dim>
Vector<dim> Vector<dim>::one() {
    Vector<dim> result;
    for (int i = 0; i < dim; ++i) {
        result.values[i] = 1.0;
    }
    return result;
}

template <int dim>
Vector<dim> Vector<dim>::zero() {
    Vector<dim> result;
    for (int i = 0; i < dim; ++i) {
        result.values[i] = 0.0;
    }
    return result;
}

template <int dim>
Vector<dim> operator*(const double other, const Vector<dim>& vec) {
    return vec * other;
}

Vector2D quadCentroid(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const Vector2D& p4) {
    return (p1 + p2 + p3 + p4) / 4.0;
}

double triangleArea(const Lin::Vector2D& p1, const Lin::Vector2D& p2, const Lin::Vector2D& p3) {
    double a = (p1 - p2).magnitude();
    double b = (p2 - p3).magnitude();
    double c = (p3 - p1).magnitude();

    double s = (a + b + c) / 2.0;
    return std::sqrt(s * (s - a) * (s - b) * (s - c));
}

double quadArea(const Vector2D& p1, const Vector2D& p2, const Vector2D& p3, const Vector2D& p4) {
    std::vector<const Vector2D*> sortedPoints = {&p1, &p2, &p3, &p4};
    std::sort(sortedPoints.begin(), sortedPoints.end(), [](const Vector2D* a, const Vector2D* b) {
        return std::atan2(a->y(), a->x()) < std::atan2(b->y(), b->x());
    });

    Vector2D d1 = *sortedPoints[0] - *sortedPoints[2];
    Vector2D d2 = *sortedPoints[1] - *sortedPoints[3];

    if (d1.magnitude() <= d2.magnitude()) {
        return triangleArea(*sortedPoints[0], *sortedPoints[1], *sortedPoints[2]) +
               triangleArea(*sortedPoints[0], *sortedPoints[2], *sortedPoints[3]);
    } else {
        return triangleArea(*sortedPoints[0], *sortedPoints[1], *sortedPoints[3]) +
               triangleArea(*sortedPoints[1], *sortedPoints[2], *sortedPoints[3]);
    }
}

}

#endif // VECTORMATH_CC
