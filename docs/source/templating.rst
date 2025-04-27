Templating
=======================

Nidhoggr uses templated classes predominantly to allow the user to specify the number of dimensions 
in a class without having to write out the same code multiple times. Most classes that are going to interact
with other templated classes from within Nidhoggr should follow the convention of using ``template <int dim>``.

For instance, ``template <int dim> class RungeKutta2Integrator : public Integrator<dim>`` is a templated RK2 
integrator class that inherits from the base integrator templated class. This way, other classes used throughout
the derived class will have the proper dimensionality, such as ``Physics<dim>``.

The other use of templates in Nidhoggr is for Field datatypes, as in ``template <typename T> class Field``. This allows
methods on the NodeList class that retrieve and store Field data to retrieve any type of Field, as in
``template <typename T> Field<T>* getField(const std::string& name) const``, and Fields can then be used as their
templated data types, as in ``VectorField* position = nodeList->getField<Vector>("position")``, where 
``using VectorField = Field<Vector>``.

.. note::
    ``Vector`` is also a templated class and is in the ``Lin`` namespace, and by ``using Vector = Lin::Vector<dim>`` 
    within our templated classes, we can
    ensure that we're using the correctly dimensioned ``Vector`` class throughout without having to invoke
    ``Field<Lin::Vector<dim>>* position = nodeList->getField<Lin::Vector<dim>>("position")``.