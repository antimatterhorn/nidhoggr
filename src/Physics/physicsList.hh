#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

#include "physicsBase.hh"
#include <vector>

namespace Physics {
template <int dim>
class PhysicsList {
private:
    std::vector<PhysicsBase<dim>*> packages;
public:
    PhysicsList() {}

    ~PhysicsList() {}

    void
    addPackage(PhysicsBase<dim>* physics) {
        packages.push_back(physics);
    }
};


}

#endif