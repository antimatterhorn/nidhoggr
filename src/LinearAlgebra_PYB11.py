from PYB11Generator import *
PYB11namespaces = ["VectorMath","TensorMath"]
PYB11includes = ['"vector_math.cc"','"tensor_math.cc"']

from vector_math import *

Vector1d = PYB11TemplateClass(Vector,
                              template_parameters = ("1"),
                              cppname = "Vector<1>",
                              pyname = "Vector1d",
                              docext = " (1D).")
Vector2d = PYB11TemplateClass(Vector,
                              template_parameters = ("2"),
                              cppname = "Vector<2>",
                              pyname = "Vector2d",
                              docext = " (2D).")
Vector3d = PYB11TemplateClass(Vector,
                              template_parameters = ("3"),
                              cppname = "Vector<3>",
                              pyname = "Vector3d",
                              docext = " (3D).") 

from tensor_math import *

Tensor1d = PYB11TemplateClass(Tensor,
                              template_parameters = ("1"),
                              cppname = "Tensor<1>",
                              pyname = "Tensor1d",
                              docext = " (1D).")
Tensor2d = PYB11TemplateClass(Tensor,
                              template_parameters = ("2"),
                              cppname = "Tensor<2>",
                              pyname = "Tensor2d",
                              docext = " (2D).")
Tensor3d = PYB11TemplateClass(Tensor,
                              template_parameters = ("3"),
                              cppname = "Tensor<3>",
                              pyname = "Tensor3d",
                              docext = " (3D).") 