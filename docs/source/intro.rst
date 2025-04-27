Introduction
============

The Níðhöggr (pronounced Nith-hewer) is the mythical serpent demon that gnaws at the roots of the world-tree, Yggdrasil. 
This naming choice may or may not have been inspired by my simultaneous loathing and admiration for tree codes.

.. image:: Nidhogg.png

Purpose of Nidhoggr
-------------------

Nidhoggr is a generic physics simulation framework. It is designed to be used as a base for varied physics simulation methods 
(FVM,FEM,etc) while keeping helper methods like equations of state and integrators generic enough to be portable between a wide 
variety of methods choices. Nidhoggr's major classes and methods are written in C++ and wrapped in Python using pybind11 to 
enable them to be imported as Python3+ modules inside a runscript. Python holds and passes the pointers to most objects 
inside the code, while the integration step is always handled by compiled C++ code. Any Python class that returns the expected 
data types of the compiled C++ classes can substitute for a precompiled package (e.g. a custom equation of state), though speed will suffer. 

Overview of Capabilities
-------------------------

Nidhoggr's capabilities as of |today| are shown below:

.. csv-table:: Nidhoggr's current capabilities
   :file: capabilities.csv
   :header-rows: 1

Intended Audience
-----------------

Nidhoggr's intended audience is computational scientists who want a toy simulation code to scope simple problems that's easily 
driveable and scriptable with a Python interface, and anyone who doesn't mind getting their hands dirty writing their own physics 
packages in a fully abstracted simulation framework.
