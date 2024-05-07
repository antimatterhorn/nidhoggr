import sys
buildir = "../build/src/"

for dir in ["Math",
            "DataBase",
            "Mesh",
            "State",
            "Physics",
            "EOS",
            "Type",
            "Integrator"]:
    sys.path.append(buildir+dir)

from Mesh import *
from DataBase import *
from LinearAlgebra import *
from Physics import *
from State import *
from EOS import *
from PhysicalConstants import *
from Units import *
from Integrator import *

print("                               ")
print("     _   _ _                  ")
print(" ___|_|_| | |_ ___ ___ ___ ___")
print("|   | | . |   | . | . | . |  _|")
print("|_|_|_|___|_|_|___|_  |_  |_|  ")
print("      v0.2.0      |___|___|    ")
print("\n")
