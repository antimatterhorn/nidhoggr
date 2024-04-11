from PYB11Generator import *

'''
@pybind11module("std_any_converter")
def std_any_converter(module):
    module.add_header("""
#include <any>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
""")

    module.add_cast("std::any",
                    to_py  = """
                      [] (const std::any& a) -> py::object {
                          try {
                              // Attempt to cast to common types
                              if (a.type() == typeid(int)) {
                                  return py::cast(std::any_cast<int>(a));
                              } else if (a.type() == typeid(double)) {
                                  return py::cast(std::any_cast<double>(a));
                              } else if (a.type() == typeid(std::string)) {
                                  return py::cast(std::any_cast<std::string>(a));
                              }
                              // Add other types as needed...
                          } catch (const std::bad_any_cast&) {
                              // If cannot cast, return None
                              return py::none();
                          }
                      }
                    """,
                    to_cpp = """
                      [] (py::handle h) -> std::any {
                          if (h.is_none()) {
                              return std::any();
                          } else if (PyLong_Check(h.ptr())) {
                              return std::any(py::cast<int>(h));
                          } else if (PyFloat_Check(h.ptr())) {
                              return std::any(py::cast<double>(h));
                          } else if (PyUnicode_Check(h.ptr())) {
                              return std::any(py::cast<std::string>(h));
                          }
                          // Add other types as needed...
                          else {
                              throw py::type_error("Invalid type for std::any conversion");
                          }
                      }
                    """)
'''

@PYB11template("dim")
class Grid:
    def pyinit1(self,nx="int",sx="double"):
        return
    def pyinit2(self,nx="int",ny="int",sx="double",sy="double"):
        return
    def pyinit3(self,nx="int",ny="int",nz="int",sx="double",sy="double",sz="double"):
        return
    @PYB11pyname("state")
    def getState(self,id="int"):
        return

    nx = PYB11property("int", getter="getnx", doc="The number of x coords.")
    ny = PYB11property("int", getter="getny", doc="The number of y coords.")
    nz = PYB11property("int", getter="getnz", doc="The number of z coords.")
    dx = PYB11property("double", getter="getdx", doc="The number of x coords.")
    dy = PYB11property("double", getter="getdy", doc="The number of y coords.")
    dz = PYB11property("double", getter="getdz", doc="The number of z coords.")