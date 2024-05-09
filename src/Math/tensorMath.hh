
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
    // Use std::array instead of std::vector
    std::array<double, dim*dim> values;

    // Constructors
    Tensor() {
        // Initialize values array based on the dimension
        for (int i = 0; i < dim*dim; ++i) {
            values[i] = 0.0;
        }
    }

    // Constructor with initializer list
    Tensor(std::initializer_list<double> init) {
        if (init.size() != dim*dim) {
            throw std::invalid_argument("Invalid number of arguments for Tensor constructor");
        }
        
        // Copy values from initializer list to array
        std::copy(init.begin(), init.end(), values.begin());
    }

    // Methods
    Tensor<dim> 
    add(const Tensor<dim>& other) const {
        Tensor<dim> result;
        for (int i = 0; i < dim*dim; ++i) {
            result.values[i] = values[i] + other.values[i];
        }
        return result;
    }

    Tensor<dim> 
    sub(const Tensor<dim>& other) const {
        Tensor<dim> result;
        for (int i = 0; i < dim*dim; ++i) {
            result.values[i] = values[i] - other.values[i];
        }
        return result;
    }

    Tensor<dim> 
    scalarProduct(double scalar) const {
        Tensor<dim> result;
        for (int i = 0; i < dim*dim; ++i) {
            result.values[i] = values[i] * scalar;
        }
        return result;
    }

    double 
    determinant() const {
        double result;
        if constexpr (dim==1)
            result = values[0];
        else if constexpr (dim==2)
            result = values[0]*values[3] - values[1]*values[2];
        else if constexpr (dim==3)
            result = xx() * (yy() * zz() - yz() * zy()) -
                     xy() * (yx() * zz() - yz() * zx()) +
                     xz() * (yx() * zy() - yy() * zx());
        return result;
    }

    // Operators
    Tensor<dim> 
    operator+(const Tensor<dim>& other) const {
        return add(other);
    }

    Tensor<dim> 
    operator-(const Tensor<dim>& other) const {
        return sub(other);
    }

    Tensor<dim> 
    operator*(const double other) const {
        return scalarProduct(other);
    }

    Vector<dim> 
    vecMul(const Vector<dim> other) const {
        Vector<dim> result = Vector<dim>();
        if (dim>1){
            std::vector<Vector<dim>> rows(dim,Vector<dim>());
            for(int i=0;i<dim;++i)
                for(int j=0;j<dim;++j)
                    rows[i][j] = values[i*dim+j];
            for(int i=0;i<dim;++i)
                result[i] = rows[i]*other;
        }
        else {
            result[0] = values[0]*other[0];
        }
        return result;
    }

    Tensor<dim> 
    operator-() const {
        return scalarProduct(-1.0);
    }

    bool 
    operator==(const Tensor<dim> other) const {
        bool result = true;
        for (int i = 0; i < dim*dim; ++i) {
            if(values[i] != other.values[i])
                result = false;
                break;
        }
        return result;
    }

    bool 
    operator!=(const Tensor<dim> other) const {
        bool result = true;
        for (int i = 0; i < dim*dim; ++i) {
            if(values[i] != other.values[i])
                result = false;
                break;
        }
        return !result;
    }

    double xx() const {
        return component(0,0);
    }

    double xy() const {
        return component(0,1);
    }

    double xz() const {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        }
        return component(0,2);
    }

    double yx() const {
        return component(1,0);
    }

    double yy() const {
        return component(1,1);
    }

    double yz() const {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        }
        return component(1,2);
    }

    double zx() const {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        }
        return component(2,0);
    }

    double zy() const {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        }
        return component(2,1);
    } 

    double zz() const {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        }
        return component(2,2);
    }

    void setxx(double val) { values[0] = val; }
    void setxy(double val) { values[1] = val; }
    void setxz(double val) { 
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        }
        values[3] = val; 
    }
    void setyx(double val) { values[dim+0] = val; }
    void setyy(double val) { values[dim+1] = val; }
    void setyz(double val) {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        } 
        values[dim+2] = val; 
    }
    void setzx(double val) {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        } 
        values[2*dim] = val; 
    }
    void setzy(double val) {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        } 
        values[2*dim+1] = val; 
    }
    void setzz(double val) {
        if (dim < 3) {
            throw std::out_of_range("Insufficient dimensionality");
        } 
        values[2*dim+2] = val; 
    }

    std::string 
    toString() const {
        std::string result = "Tensor " + std::to_string(dim) + "x" + std::to_string(dim) + ":\n";

        for (int i = 0; i < dim; ++i) {
            result += "  ";
            for (int j = 0; j < dim; ++j) {
                result += std::to_string(values[i*dim+j]);
                if (j < dim - 1) {
                    result += ", ";
                }
            }
            result += "\n";
        }

        return result;
    }
    

private:
    double component(int i,int j) const { return values[i*dim+j]; }
};

using Tensor1D = Tensor<1>;
using Tensor2D = Tensor<2>;
using Tensor3D = Tensor<3>;

template <int dim>
Tensor<dim> 
operator*(const double other, const Tensor<dim>& tens) {
    return tens*other;
}
}

#endif // TENSORMATH_HH