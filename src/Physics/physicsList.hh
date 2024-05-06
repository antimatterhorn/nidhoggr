#ifndef PHYSICSLIST_HH
#define PHYSICSLIST_HH

#include "physics.hh"
#include <vector>

template <int dim>
class PhysicsList {
private:
    std::vector<Physics<dim>*> packages;
public:
    PhysicsList() {}

    ~PhysicsList() {}

    void
    addPackage(Physics<dim>* physics) {
        packages.push_back(physics);
    }
};


#endif