Advanced Topics
===============

Periodic Work
--------------------
Nidhoggr's controller object permits for periodic work to be inserted at a chosen cadence into the simulation.
This is done by passing a ``periodicWork`` argument to the controller constructor: 

.. code-block:: python
    
    controller = Controller(integrator=integrator,
                            statStep=100,
                            periodicWork=periodicWork)

Suppose, for instance, I want to insert periodic work that oscillates the value of my ``phi`` field at a certain cadence in the center
of the grid domain.
I can do this by creating an oscillator class in Python:

.. literalinclude:: oscillator.py
   :language: python

Then I construct the associated periodic work object like so:

.. code-block:: python

    osc = oscillate(nodeList=myNodeList,grid=grid,cs=cs,width=nx,height=ny)
    periodicWork = [osc]

before passing this Python list to the controller constructor. The controller expects a ``__call__(self,cycle,time,dt)`` method inside 
each periodic work function and will invoke each ``call`` method at your chosen cadence. You can use periodic work to implement many other 
effects in your simulation, or to perform analysis tasks in real time during integration. 

Calculators and Linear Algebra
------------------------------
Nidhoggr also includes a small number of calculator objects that can be used anywhere in a Nidhoggr
script. These includes

.. code-block:: text

    cosmology
    stringArt
    timeDilation

Each of these has an example script in the ``examples`` directory.

In addition to the above-mentioned calculators, Nidhoggr includes a fairly well-featured linear
algebra computational package through its Vector and Tensor class methods. 

.. note::
    Nihoggr's Vector and Tensor classes are limited to a maximum of three dimensions along any
    span.

Implicit vs. Explicit Time integration
---------------------------------------
Most of the physics packages in Nidhoggr are designed for use with explicit time integration
and can become quite unstable if you try to run them with implicit time integration (e.g. Crank-Nicolson).
As of version 0.8.5, the only physics module that is stable under implicit time integration is ImplicitPhysicsXd. 