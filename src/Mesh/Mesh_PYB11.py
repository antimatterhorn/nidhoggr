from PYB11Generator import *
PYB11namespaces = ["Mesh"]
PYB11includes = ['"grid.hh"']

from grid import *

Grid1d = PYB11TemplateClass(Grid,
                              template_parameters = ("1"),
                              cppname = "Grid<1>",
                              pyname = "Grid1d",
                              docext = " (1D).")
Grid2d = PYB11TemplateClass(Grid,
                              template_parameters = ("2"),
                              cppname = "Grid<2>",
                              pyname = "Grid2d",
                              docext = " (2D).")
Grid3d = PYB11TemplateClass(Grid,
                              template_parameters = ("3"),
                              cppname = "Grid<3>",
                              pyname = "Grid3d",
                              docext = " (3D).") 