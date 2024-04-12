namespace Hydro {
template <int dim>
class HydroState {
public:
    double density;
    std::array<double, dim> momentum;
    double energy;

    HydroState(double rho = 0.0, const std::array<double, dim>& mom = {}, double E = 0.0) 
        : density(rho), momentum(mom), energy(E) {}

    ~HydroState() {}
};
}
