Core Concepts
=============



Units and Constants
-------------------

Nidhoggr is unit agnostic. That is to say, the code does not *prescribe* any particular units for you. However, 
for most problems, you will want to define your units in order for certain universal constants like $G$ to have their 
correct values. This is done via the PhysicalConstants object which has two constructor methods. You can either supply 
the full scope of your desired units with unit length (in meters), unit mass (in kg), 
unit time (in seconds), unit temperature (in Kelvin), and unit charge (in Coulomb), or just a subset of the first three, 
wherein temperature will be assumed to be Kelvins and charge will be assumed to be Coulombs. 

For example, in order to simulate something like the Earth with state quantities near 1, you may choose to instantiate your units like so:

``PhysicalConstants(6.387e6, 5.97e24, 1.0)``

From these units, Nidhoggr will calculate at the time of the constructor new values for all of the universal constants 
to use in your chosen physics packages.

Nidhoggr also comes with some helper methods for a handful of frequently used unit systems in 
``Units.py``, like ``MKS()``, ``CGS()``, and ``SOL()``. 
Simply invoke them with ``myUnits = MKS()`` if you've imported the ``Units`` module.


Nodelists and Fields
--------------------
Fields are essentially decorated std::vectors that come with some additional functionality that make them 
assignable to various physics packages by name, *e.g.* ``density`` may be a Field that certain physics classes create and evolve
as part of the state.
Nidhoggr's primary container for state data is the ``Nodelist`` class. Fields are assigned to a ``Nodelist`` for containerization
of data and state copying.

Physics Packages
--------------------
Nidhoggr's integrators expect assigned (derived) physics classes to override the physics base class methods for 
PrestepInitialize, EvaluateDerivatives and FinalizeStep. The order in which you assign these physics objects to 
the integrator is the order in which they will be computed (operator splitting). Consult the Classes.md files in 
``src`` subdirectories for interface guides on many of the classes and methods.

Equations of State
--------------------



Mesh/Grid Handling
--------------------


Boundary Conditions
--------------------

Integrators
--------------------

The Controller
--------------------

Periodic Work
--------------------
