//include "physics.hh"
//include "../Mesh/grid.hh"
//include <iostream>

template <int dim>
class WaveEquation : public Physics<dim> {
protected:
    Mesh::Grid<dim> grid;
public:
    WaveEquation() {}

    WaveEquation(NodeList* nodeList, PhysicalConstants& constants, Mesh::Grid<dim>& grid) : 
        Physics<dim>(nodeList,constants),
        grid(grid) {

        int numNodes = nodeList->size();

    }

    ~WaveEquation() {}

// d^2 phi / dt^2 = c^2 del^2 phi

// IIRC, which solution you get all comes down to initial conditions
// and boundary conditions.

// RHS is second spatial derivative.  Easy.  From Taylor,
// phi_{i+1,j} = phi_ij + dx * phi,x_ij + dx^2/2 phi,,x_ij + ... // where ,,x means d^2/dx^2
// phi_{i-1,j} = phi_ij - dx * phi,x_ij + dx^2/2 phi,,x_ij + ...
// phi_{i,j+1} = phi_ij + dy * phi,y_ij + dy^2/2 phi,,y_ij + ... // where ,,y means d^2/dy^2
// phi_{i,j-1} = phi_ij - dy * phi,y_ij + dy^2/2 phi,,y_ij + ...
// --------- = -----------------------------------
//           = 4 * phi_ij          + 2 * dx^2/2 phi,,x_ij + 2 * dy^2/2 phi,,y_ij + ...
//           = 4 * phi_ij          + dx^2 phi,,x_ij + dy^2 phi,,y_ij + ...
//           = 4 * phi_ij          + dx^2 (phi,,x_ij + phi,,y_ij) + ... // assume dx=dy
//           = 4 * phi_ij          + dx^2 del^2(phi_ij) + ... // assume dx=dy

// del^2 phi_ij = (-4*phi_{i,j} + phi_{i+1,j} + phi_{i-1,j} phi_{i,j+1} + phi_{i,j-1})/dx^2

// Time derivative gives us many (sigh, many many) options.  Does this
// need to be implicit?  I dunno.  Let's start explicit and see what
// the issues are.

// split time derivative into two steps
// d phi / dt = xi
// d^2 phi / dt^2 = d xi / dt
// d xi / dt = c^2 del^2 phi
// evolve xi and then use xi to evolve phi

// Let's go with forward Euler (omg yuck) for the first version!  Super
// easy!

// xi^{n+1}_i - xi^{n}_i = dt * c^2 * del^2 phi^n_ij
// phi^{n+1}_i - phi^{n}_i = dt * xi^{n+1}_ij


// boundary conditions... reflecting for all boundaries seems fine to start with.
// We can put a forcing function in the middle to start.

};
