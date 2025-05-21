Class Guides
=======================
Most classes in Nidhoggr are purely virtual. This allows for the interaction between class species (like physics classes
and integrators) to be abstracted and predictable.  

Physics Classes
--------------------
The primary class that you'll interact with in Nidhoggr is the ``Physics`` class. This class contains the prescription
for the state vector and all of the necessary logic for computing derivatives of the state vector.
Nidhoggr's integrators expect assigned (derived) physics classes to override the physics base class methods for 
``PrestepInitialize``, ``EvaluateDerivatives``, and ``FinalizeStep``. 

Let's use ``constantGravity.cc`` in the ``src/Physics`` directory as an example of a derived class that implements these methods.

.. literalinclude:: ../../src/Physics/constantGravity.cc
   :language: c++
   :linenos:
   :lines: 4-29
   :lineno-start: 4

In the constructor for ``ConstantGravity``, we pass in the ``NodeList`` pointer, a ``PhysicalConstants`` object, and a ``Vector`` that
prescribes the direction and magnitude of the acceleration. Then we check for the
existence of certain Fields on the NodeList that are required by this particular physics class, and if they don't exist, we
create them on the NodeList with ``nodeList->insertField<T>(name)``, in this case ``acceleration`` which is a ``Vector``. 

Next we grab the ``State<dim>`` object from the base physics class and assign the ``position`` and ``velocity`` Fields to it. 
In the case of 
the constant gravity package, the only derivatives are the position derivative and velocity derivative, and as a standard, we keep
antiderivatives on the ``State`` object (to be preserved as copies) and derivatives on the ``NodeList`` object (to be altered in series
by successive physics packages). For that reason, we do not assign ``acceleration`` to the ``State`` object for this physics class.
You can think of ``State`` Fields as the independent variables that are being solved for at the next time step, and their derivatives 
are dependent variables that change within a step.

.. literalinclude:: ../../src/Physics/constantGravity.cc
   :language: c++
   :linenos:
   :lines: 33-38
   :lineno-start: 33

The PrestepInitialize method is used to initialize the ``State`` object at the beginning of a time step. In this case, 
``ConstantGravity`` is simply updating the ``State`` to the current values of the ``NodeList``.

.. literalinclude:: ../../src/Physics/constantGravity.cc
   :language: c++
   :linenos:
   :lines: 40-68
   :lineno-start: 40

``EvaluateDerivatives`` is used to compute the derivatives of the ``State`` object at each node. In this case, we are computing the 
change in velocity and position due to a constant acceleration, so we have a pair of ODEs:

.. math::
    \begin{aligned}
    \dot{x} &= v(t) + a \Delta t\\
    \dot{v} &= a\\
    \end{aligned}

The integrator invokes this method with intial and derivatives ``State`` vectors and a time step, and the physics class 
stores the value of those derivatives back to the ``deriv`` ``State`` object.

.. note::
    The integrator will call this method multiple times per time step if the particular integrator in question is of high temporal order. 
    Thus, the value of ``dt`` here may be a partial step and the initial ``State`` object may change within a time step, 
    but in the case of a simple, forward Euler integration (:math:`\mathcal{O}(\Delta t)`), ``dt=0`` means that we are 
    computing the derivatives using ``State`` vector quantities evaluated at the current time. 

The final bit of code in ``EvaluateDerivatives`` merely calculates the minimum timestep based on a velocity condition.

.. literalinclude:: ../../src/Physics/constantGravity.cc
   :language: c++
   :linenos:
   :lines: 77-104
   :lineno-start: 75

``FinalizeStep`` is meant to tie up any further calculations that should happen at the end of a time step. In this case, we merely push
the values of the ``finalState`` passed from the integrator to the physics ``State`` object and the corresponding Fields in the ``NodeList``
using the ``Field.copyValues`` method.

Integrator Classes
------------------
Nihoggr's integrators follow a basic pattern of initializing the state of the physics objects at each step, applying boundary conditions,
evaluating derivatives, advancing the state of each physics object from those derivatives, and then finalizing each physics object's state.
An example of this pattern for simple forward Euler integration is shown below:

.. literalinclude:: ../../src/Integrators/integrator.cc
   :language: c++
   :lines: 6-66

.. note::
   While this class is a foward Euler integrator, it is also the base class for all integrators in Nidhoggr.

Equations of State
------------------
Equations of state are possibly the simplest classes in Nidhoggr. They consume Field objects (or referenced doubles) 
and set the values of other Fields according to their respective closure equations. Most of the logic for how they 
work is self-described by the base class interface file ``equationOfState.hh``.

.. literalinclude:: ../../src/EOS/equationOfState.hh
   :language: c++
   :linenos:
   :lines: 9-48
   :lineno-start: 9

.. note::
    Equations of state are not typically templated in Nidhoggr since they act only on scalar Fields. 