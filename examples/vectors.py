from nidhoggr import *

vec1 = Vector3d(1.0,2.0,3.0)
vec2 = Vector3d(2.0,3.0,4.0)

print("a\t=",   vec1)
print("b\t=",   vec2)
print("a*b\t=", vec1*vec2)
print("axb\t=", vec1.cross(vec2))
print("a+b\t=", vec1+vec2)
print("a*2\t=", vec1*2.0)
print("2*a\t=", 2.0*vec1)
print("|a|\t=", vec1.magnitude)
print("a.norm\t=",vec1.normal())