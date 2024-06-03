#ifndef TENSORMATH_CC
#define TENSORMATH_CC

#include "tensorMath.hh"

namespace Lin {

template <int dim>
Tensor<dim>::Tensor() {
    for (int i = 0; i < dim*dim; ++i)
        values[i] = 0.0;
}

template <int dim>
Tensor<dim>::Tensor(std::initializer_list<double> init) {
    if (init.size() != dim*dim)
        throw std::invalid_argument("Invalid number of arguments for Tensor constructor");      
    std::copy(init.begin(), init.end(), values.begin());
}

template <int dim>
Tensor<dim> Tensor<dim>::add(const Tensor<dim>& other) const {
    Tensor<dim> result;
    for (int i = 0; i < dim*dim; ++i)
        result.values[i] = values[i] + other.values[i];
    return result;
}

template <int dim>
Tensor<dim> Tensor<dim>::sub(const Tensor<dim>& other) const {
    Tensor<dim> result;
    for (int i = 0; i < dim*dim; ++i)
        result.values[i] = values[i] - other.values[i];
    return result;
}

template <int dim>
Tensor<dim> Tensor<dim>::scalarProduct(double scalar) const {
    Tensor<dim> result;
    for (int i = 0; i < dim*dim; ++i)
        result.values[i] = values[i] * scalar;
    return result;
}

template <int dim>
double Tensor<dim>::determinant() const {
    double result;
    if constexpr (dim == 1)
        result = values[0];
    else if constexpr (dim == 2)
        result = values[0]*values[3] - values[1]*values[2];
    else if constexpr (dim == 3)
        result = xx() * (yy() * zz() - yz() * zy()) -
                 xy() * (yx() * zz() - yz() * zx()) +
                 xz() * (yx() * zy() - yy() * zx());
    return result;
}

template <int dim>
Tensor<dim> Tensor<dim>::operator+(const Tensor<dim>& other) const {
    return add(other);
}

template <int dim>
Tensor<dim> Tensor<dim>::operator-(const Tensor<dim>& other) const {
    return sub(other);
}

template <int dim>
Tensor<dim> Tensor<dim>::operator*(const double other) const {
    return scalarProduct(other);
}

template <int dim>
Vector<dim> Tensor<dim>::vecMul(const Vector<dim> other) const {
    Vector<dim> result = Vector<dim>();
    if (dim > 1) {
        std::vector<Vector<dim>> rows(dim, Vector<dim>());
        for (int i = 0; i < dim; ++i)
            for (int j = 0; j < dim; ++j)
                rows[i][j] = values[i*dim + j];
        for (int i = 0; i < dim; ++i)
            result[i] = rows[i] * other;
    } else {
        result[0] = values[0] * other[0];
    }
    return result;
}

template <int dim>
Tensor<dim> Tensor<dim>::operator-() const {
    return scalarProduct(-1.0);
}

template <int dim>
bool Tensor<dim>::operator==(const Tensor<dim> other) const {
    bool result = true;
    for (int i = 0; i < dim*dim; ++i) {
        if (values[i] != other.values[i]) {
            result = false;
            break;
        }
    }
    return result;
}

template <int dim>
bool Tensor<dim>::operator!=(const Tensor<dim> other) const {
    bool result = true;
    for (int i = 0; i < dim*dim; ++i) {
        if (values[i] != other.values[i]) {
            result = false;
            break;
        }
    }
    return !result;
}

template <int dim>
double Tensor<dim>::xx() const {
    return component(0, 0);
}

template <int dim>
double Tensor<dim>::xy() const {
    return component(0, 1);
}

template <int dim>
double Tensor<dim>::xz() const {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    return component(0, 2);
}

template <int dim>
double Tensor<dim>::yx() const {
    return component(1, 0);
}

template <int dim>
double Tensor<dim>::yy() const {
    return component(1, 1);
}

template <int dim>
double Tensor<dim>::yz() const {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    return component(1, 2);
}

template <int dim>
double Tensor<dim>::zx() const {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    return component(2, 0);
}

template <int dim>
double Tensor<dim>::zy() const {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    return component(2, 1);
}

template <int dim>
double Tensor<dim>::zz() const {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    return component(2, 2);
}

template <int dim>
void Tensor<dim>::setxx(double val) { values[0] = val; }

template <int dim>
void Tensor<dim>::setxy(double val) { values[1] = val; }

template <int dim>
void Tensor<dim>::setxz(double val) {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    values[2] = val;
}

template <int dim>
void Tensor<dim>::setyx(double val) { values[dim + 0] = val; }

template <int dim>
void Tensor<dim>::setyy(double val) { values[dim + 1] = val; }

template <int dim>
void Tensor<dim>::setyz(double val) {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    values[dim + 2] = val;
}

template <int dim>
void Tensor<dim>::setzx(double val) {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    values[2 * dim] = val;
}

template <int dim>
void Tensor<dim>::setzy(double val) {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    values[2 * dim + 1] = val;
}

template <int dim>
void Tensor<dim>::setzz(double val) {
    if (dim < 3)
        throw std::out_of_range("Insufficient dimensionality");
    values[2 * dim + 2] = val;
}

template <int dim>
Tensor<dim> Tensor<dim>::one() {
    Tensor<dim> result = Tensor<dim>();
    for (int i = 0; i < dim; ++i) {
        result.values[i*dim + i] = 1.0;
    }
    return result;
}

template <int dim>
Tensor<dim> Tensor<dim>::zero() {
    Tensor<dim> result = Tensor<dim>();
    return result;
}

template <int dim>
std::string Tensor<dim>::toString() const {
    std::string result = "Tensor " + std::to_string(dim) + "x" + std::to_string(dim) + ":\n";
    for (int i = 0; i < dim; ++i) {
        result += "  ";
        for (int j = 0; j < dim; ++j) {
            result += std::to_string(values[i*dim + j]);
            if (j < dim - 1) {
                result += ", ";
            }
        }
        result += "\n";
    }
    return result;
}

template <int dim>
double Tensor<dim>::component(int i, int j) const {
    return values[i * dim + j];
}

template <int dim>
Tensor<dim> operator*(const double other, const Tensor<dim>& tens) {
    return tens * other;
}

} // namespace Lin

#endif