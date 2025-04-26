Core Concepts
=============

Dimensionality
--------------

Nidhoggr is designed from the start to be extensible to multiple spatial dimensions. This is accomplished by templating
most classes on integer dimensionality. For example, 

``template <int dim>
class Kinetics : public Physics<dim> {}``

defines a derived class ``Kinetics`` that operates in 1, 2, or 3 spaital dimensions. When templated classes are Python wrapped in Nidhoggr,
they typically specify dimensionality in their instatiated name, *e.g.* ``Kinetics2D``.

Units and Constants
-------------------

Nidhoggr is unit agnostic. That is to say, the code does not *prescribe* any particular units for you. However, 
for most problems, you will want to define your units in order for certain universal constants like *G* to have their 
correct values. This is done via the ``PhysicalConstants`` object which has two constructor methods. You can either supply 
the full scope of your desired units with unit length (in meters), unit mass (in kg), 
unit time (in seconds), unit temperature (in Kelvin), and unit charge (in Coulomb), or just a subset of the first three, 
wherein temperature will be assumed to be Kelvins and charge will be assumed to be Coulombs. 

For example, in order to simulate something like the Earth with state quantities near 1, you may choose to instantiate your units like so:

``myUnits = PhysicalConstants(6.387e6, 5.97e24, 1.0)``

From these units, Nidhoggr will calculate at the time of the constructor new values for all of the universal constants 
to use in your chosen physics packages.

Nidhoggr also comes with some helper methods for a handful of frequently used unit systems in 
``Units.py``, like ``MKS()``, ``CGS()``, and ``SOL()``. 
Simply invoke them with ``myUnits = MKS()`` if you've imported the ``Units`` module.

The ``PhysicalConstants`` object is typically passed to a physics package via the constructor for that package. Not all physics packages
require a ``PhysicalConstants`` object.

Nodelists and Fields
--------------------
Fields are essentially decorated std::vectors that come with some additional functionality that make them 
assignable to various physics packages by name, *e.g.* ``density`` may be a Field that certain physics classes create and evolve
as part of the state. 

Nidhoggr's primary container for state data is the ``Nodelist`` class. Fields are assigned to a ``Nodelist`` for containerization
of data, for pairing of different Field data into a state, and for state copying. Most physics packages will expect a ``Nodelist`` 
to be passed as a constructor argument to keep track of
which state vectors the physics package is intended to evolve. 

Note that Nidhoggr does not assume any specific Fields (except ``id``) are necessary without a physics package first 
creating that Field and then assigning it to the ``Nodelist``, not even ``position``.

Physics Packages
--------------------
Nidhoggr's integrators expect assigned (derived) physics classes to override the physics base class methods for 
PrestepInitialize, EvaluateDerivatives and FinalizeStep. Assigning multiple physics packages to
the integrator is as simple as passing a Python list of constructed physics objects to the integrator's constructor. 
The order in which you assign these physics objects to 
the integrator is the order in which they will be computed (operator splitting). For example,
suppose I've created a ``Godzilla`` physics object and separately a ``KingKong`` physics object, and I want my RK4 integrator to 
evolve them in that order, I'd construct the RK4 integrator as ``RungeKutta4Integrator2d(packages=[Godzilla,KingKong],dtmin=0.01)``,
or something to that effect.


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
