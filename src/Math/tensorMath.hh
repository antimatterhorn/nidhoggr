#ifndef TENSORMATH_HH
#define TENSORMATH_HH

#include <string>
#include <vector>
#include <array>
#include <cmath>
#include <algorithm>
#include "vectorMath.hh"

namespace Lin {

template <int dim>
class Tensor {
public:
    std::array<double, dim*dim> values;

    Tensor();
    Tensor(std::initializer_list<double> init);

    Tensor<dim> add(const Tensor<dim>& other) const;
    Tensor<dim> sub(const Tensor<dim>& other) const;
    Tensor<dim> scalarProduct(double scalar) const;
    double determinant() const;
    Tensor<dim> operator+(const Tensor<dim>& other) const;
    Tensor<dim> operator-(const Tensor<dim>& other) const;
    Tensor<dim> operator*(const double other) const;
    Vector<dim> vecMul(const Vector<dim> other) const;
    Tensor<dim> operator-() const;
    bool operator==(const Tensor<dim> other) const;
    bool operator!=(const Tensor<dim> other) const;

    double xx() const;
    double xy() const;
    double xz() const;
    double yx() const;
    double yy() const;
    double yz() const;
    double zx() const;
    double zy() const;
    double zz() const;

    void setxx(double val);
    void setxy(double val);
    void setxz(double val);
    void setyx(double val);
    void setyy(double val);
    void setyz(double val);
    void setzx(double val);
    void setzy(double val);
    void setzz(double val);

    static Tensor<dim> one();
    static Tensor<dim> zero();

    std::string toString() const;

private:
    double component(int i, int j) const;
};

using Tensor1D = Tensor<1>;
using Tensor2D = Tensor<2>;
using Tensor3D = Tensor<3>;

template <int dim>
Tensor<dim> operator*(const double other, const Tensor<dim>& tens);

} // namespace Lin

#include "tensorMath.cc"

#endif // TENSORMATH_HH
