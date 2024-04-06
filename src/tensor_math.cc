#include <string>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace TensorMath {
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
            assignComponents();
        }

        // Constructor with initializer list
        Tensor(std::initializer_list<double> init) {
            if (init.size() != dim*dim) {
                throw std::invalid_argument("Invalid number of arguments for Tensor constructor");
            }
            
            // Copy values from initializer list to array
            std::copy(init.begin(), init.end(), values.begin());
            assignComponents();
        }

        // Methods
        Tensor<dim> add(const Tensor<dim>& other) const {
            Tensor<dim> result;
            for (int i = 0; i < dim*dim; ++i) {
                result.values[i] = values[i] + other.values[i];
            }
            result.assignComponents();
            return result;
        }

        Tensor<dim> sub(const Tensor<dim>& other) const {
            Tensor<dim> result;
            for (int i = 0; i < dim*dim; ++i) {
                result.values[i] = values[i] - other.values[i];
            }
            result.assignComponents();
            return result;
        }

        Tensor<dim> scalarProduct(double scalar) const {
            Tensor<dim> result;
            for (int i = 0; i < dim*dim; ++i) {
                result.values[i] = values[i] * scalar;
            }
            result.assignComponents();
            return result;
        }

        double determinant() const {
            double result;
            if constexpr (dim==1)
                result = values[0];
            else if constexpr (dim==2)
                result = values[0]*values[3] - values[1]*values[2];
            else if constexpr (dim==3)
                result = components[0][0] * (components[1][1] * components[2][2] - components[1][2] * components[2][1]) -
                         components[0][1] * (components[1][0] * components[2][2] - components[1][2] * components[2][0]) +
                         components[0][2] * (components[1][0] * components[2][1] - components[1][1] * components[2][0]);
            return result;
        }

        // Operators
        Tensor<dim> operator+(const Tensor<dim>& other) const {
            return add(other);
        }

        Tensor<dim> operator-(const Tensor<dim>& other) const {
            return sub(other);
        }

        Tensor<dim> operator*(const double other) const {
            return scalarProduct(other);
        }

        Tensor<dim> operator-() const {
            return scalarProduct(-1.0);
        }

        bool operator==(const Tensor<dim> other) const {
            bool result = true;
            for (int i = 0; i < dim*dim; ++i) {
                if(values[i] != other.values[i])
                    result = false;
                    break;
            }
            return result;
        }

        bool operator!=(const Tensor<dim> other) const {
            bool result = true;
            for (int i = 0; i < dim*dim; ++i) {
                if(values[i] != other.values[i])
                    result = false;
                    break;
            }
            return !result;
        }


        // Getter methods for individual components
        /*
        components[0][0]: xx component
        components[0][1]: xy component
        components[0][2]: xz component
        components[1][0]: yx component
        components[1][1]: yy component
        components[1][2]: yz component
        components[2][0]: zx component
        components[2][1]: zy component
        components[2][2]: zz component     
        */

        double xx() const {
            return components[0][0];
        }

        double xy() const {
            return components[0][1];
        }

        double xz() const {
            if (dim < 3) {
                throw std::out_of_range("Insufficient dimensionality");
            }
            return components[0][2];
        }

        double yx() const {
            return components[1][0];
        }

        double yy() const {
            return components[1][1];
        }

        double yz() const {
            if (dim < 3) {
                throw std::out_of_range("Insufficient dimensionality");
            }
            return components[1][2];
        }

        double zx() const {
            if (dim < 3) {
                throw std::out_of_range("Insufficient dimensionality");
            }
            return components[2][0];
        }

        double zy() const {
            if (dim < 3) {
                throw std::out_of_range("Insufficient dimensionality");
            }
            return components[2][1];
        } 

        double zz() const {
            if (dim < 3) {
                throw std::out_of_range("Insufficient dimensionality");
            }
            return components[2][2];
        }

        std::string toString() const {
            std::string result = "Tensor " + std::to_string(dim) + "x" + std::to_string(dim) + ":\n";

            for (int i = 0; i < dim; ++i) {
                result += "  ";
                for (int j = 0; j < dim; ++j) {
                    result += std::to_string(components[i][j]);
                    if (j < dim - 1) {
                        result += ", ";
                    }
                }
                result += "\n";
            }

            return result;
        }

    private:
        std::array<std::array<double, dim>, dim> components;

        void assignComponents()
        {
            for (int i = 0; i < dim; ++i) {
                for (int j = 0; j < dim; ++j) {
                    components[i][j] = values[i * dim + j];
                }
            }
        }
    };

    using Tensor1D = Tensor<1>;
    using Tensor2D = Tensor<2>;
    using Tensor3D = Tensor<3>;

    template <int dim>
    Tensor<dim> operator*(const double other, const Tensor<dim>& tens) {
        return tens*other;
    }
}