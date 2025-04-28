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