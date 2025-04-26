Core Concepts
=============

As previously described, Nidhoggr is a compiled C++ codebase that is driven primarily by Python via 
compiled Python modules. If you're familiar with how NumPy works, you have most of the knowledge you'll 
need to understand how Nidhoggr works. In fact, while Nidhoggr does have many linear algebra classes and 
methods builtin, it often works best when paired with other popular Python modules, like NumPy and Matplotlib.

Importing Nidhoggr Modules
--------------------------

The ``nidhoggr.py`` file in the tests and examples directories imports all of the compiled Nidhoggr 
modules for you, and so if you wish to import the entire codebase, you can simply ``from nidhoggr import *`` 
However, you may choose to import only a subset of the available modules for your specific problem. Consult ``nidhoggr.py`` 
for the full list of modules.

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

- Physics methods
- Equations of State
- Mesh/grid handling
- Boundary conditions
- Integrators
- Controller
- Periodic work
