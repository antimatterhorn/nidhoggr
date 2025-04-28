Core Concepts
=============

Dimensionality
--------------

Nidhoggr is designed from the start to be extensible to multiple spatial dimensions. This is accomplished by templating
most classes on integer dimensionality. For example, 

.. code-block:: c++
    
    template <int dim>
    class Kinetics : public Physics<dim> {}

defines a derived class ``Kinetics`` that operates in 1, 2, or 3 spaital dimensions. When templated classes are Python wrapped in Nidhoggr,
they typically specify dimensionality in their instantiated name, *e.g.* ``Kinetics2d``.

Units and Constants
-------------------

Nidhoggr is unit agnostic. That is to say, the code does not *prescribe* any particular units for you. However, 
for most problems, you will want to define your units in order for certain universal constants like *G* to have their 
correct values. This is done via the ``PhysicalConstants`` object which has two constructor methods. You can either supply 
the full scope of your desired units with unit length (in meters), unit mass (in kg), 
unit time (in seconds), unit temperature (in Kelvin), and unit charge (in Coulomb), or just a subset of the first three, 
whereupon temperature will be assumed to be Kelvins and charge will be assumed to be Coulombs. 

For example, in order to simulate something like the Earth with state quantities near 1, you may choose to instantiate your units like so:

.. code-block:: python

    myUnits = PhysicalConstants(6.387e6, 5.97e24, 1.0)

From these units, Nidhoggr will calculate at the time of the constructor new values for all of the universal constants 
to use in your chosen physics packages.

Nidhoggr also comes with some helper methods for a handful of frequently used unit systems in 
``Units.py``, like ``MKS()``, ``CGS()``, and ``SOL()``. 
Simply invoke them with ``myUnits = MKS()`` if you've imported the ``Units`` module.

.. note::
    The ``PhysicalConstants`` object is typically passed to a physics package via the constructor for that package. Not all physics packages
    require a ``PhysicalConstants`` object.

Fields and Nodelists
--------------------
Fields are essentially decorated std::vectors that come with some additional functionality that make them 
assignable to various physics packages by name, *e.g.* ``density`` may be a Field that certain physics classes create and evolve
as part of the state. This way, if multiple physics packages use the same Field names, they will be able to operate on the same data in
an operator split mode.

.. note::
    Nidhoggr does not currently support the use of multiple Field objects with the same internal name. If you try to assign two Fields 
    with the same name to a ``Nodelist``, Nidhoggr will raise an error.

Nidhoggr's primary container for state data is the ``Nodelist`` class. Fields are assigned to a ``Nodelist`` for containerization
of data, for pairing of different Field data into a state, and for state copying. Most physics packages will expect a ``Nodelist`` 
to be passed as a constructor argument to keep track of
which state vectors the physics package is intended to evolve. 

.. note::
    Nidhoggr does not assume any specific Fields (except ``id``) are necessary without a physics package first 
    creating that Field and then assigning it to the ``Nodelist``. If you try to access a Field called ``density`` inside
    a problem without any physics classes that use ``density``, Nidhoggr will raise an error, unless you have manually created that Field yourself
    within your problem script.

Physics Packages
--------------------
Assigning multiple physics packages to
the integrator is as simple as passing a Python list of constructed physics objects to the integrator's constructor. 
The order in which you assign these physics objects to 
the integrator is the order in which they will be computed (operator splitting). For example,
suppose I've created a ``Godzilla`` physics object and separately a ``KingKong`` physics object, and I want my RK4 integrator to 
evolve them in that order, I'd construct the RK4 integrator as ``RungeKutta4Integrator2d(packages=[Godzilla,KingKong],dtmin=0.01)``,
or something to that effect.


Equations of State
--------------------
If a particular physics package requires an equation of state (EOS), you'll need to pass it in as the ``eos`` argument. For example, suppose
I want to use an Ideal Gas EOS for my hydro physics object. I simply create the EOS with ``eos = IdealGasEOS(5.0/3.0,constants)`` 
and pass it to my hydro physics object as the ``eos`` argument: 
``hydro = GridHydroHLL2d(myNodeList,constants,eos,myGrid)``.


Mesh/Grid Handling
--------------------
<wip>

Boundary Conditions
--------------------
Currently, Nidhoggr has two species of boundary objects: grid boundaries and collider boundaries.
Grid boundaries apply to mesh-based physics and collider boundaries apply to lagrangian particles.

Grid Boundaries
^^^^^^^^^^^^^^^
The types of grid boundaries used in Nidhoggr are:

.. code-block:: text
    
    DirichletGridBoundaries
    OutflowGridBoundaries
    PeriodicGridBoundaries
    ReflectingGridBoundaries 

Each of these is available in any of 1d, 2d, or 3d varieties. Grid boundaries are applied directly
to the grid cells and so must be pointed at the correct grid object at construction.

.. code-block:: python

    box = DirichletGridBoundaries2d(grid=myGrid)

Dirichlet grid boundaries have methods for applying boundaries to cells within the grid such as 
``box.addBox(Vector2d(16,20),Vector2d(20,44))`` which would add Dirichlet conditions to the cells
whose positions span 16-20 in *x* and 20-44 in *y*.
The full list of available methods for ``DirichletGridBoundaries`` is given below.

.. literalinclude:: ../../src/Boundaries/dirichletGridBoundaries.py
    :language: python
    :lines: 8-17

The ``addDomain`` method applies Dirichlet conditions to all of the bounds (left-most,right-most,etc.)
of the mesh. The other grid boundary types merely perform ``addDomain`` at constructor time and do not
have any special methods.

.. warning::
    At this time, Nidhoggr's reflecting, periodic, and outlfow boundaries apply to all of the bounds
    of your mesh, *i.e* the left and right-most cells will be periodic as well as the top and bottom-most
    cells in 2d. 

Collider Boundaries
^^^^^^^^^^^^^^^^^^^
Collider boundaries are spherical (or round in 2d) or box-shaped objects that reflect the motion of 
lagrangian particles across the normal of their surface. A ``SphereCollider`` takes as arguments 
a position Vector for the center of the sphere (or circle), a radius, and an elasticity parameter
between 0 and 1. A value of 1 for this parameter means purely elastic collisions with absolute
momentum conservation. 

.. code-block:: python

    SphereCollider2d(position=Vector2d(x, y), radius=collider_radius, elasticity=0.8)

A ``BoxCollider`` takes as arguments two position Vectors that describe 
opposite corners of the box and an elacsticity parameter. 

.. code-block:: python

    BoxCollider2d(position1=Vector2d(x, y), position2=Vector2d(x+width,y+height), elasticity=0.5)

Collider boundaries can be assigned to lagrangian physics packages with the ``addBoundary()`` method:

.. code-block:: python

    myKineticsPhysicsPkg.addBoundary(myBoxCollider)

Integrators
--------------------
<wip>

The Controller
--------------------
The controller object is a Python class that issues instructions to the ingetrator object and performs any extra periodic work. Integration steps
are performed using the ``Step()`` method of the integrator, and the controller is responsible for issuing these steps and halting the simulation
if a ``tstop`` is supplied. The controller is simple enough to be reproduced in its entirety below:

.. literalinclude:: ../../src/Utilities/Controller.py
   :language: python
   