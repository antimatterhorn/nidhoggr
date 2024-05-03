import sys
sys.path.append("../build/src/Mesh")
sys.path.append("../build/src/Physics")
sys.path.append("../build/src/Utilities")

from Mesh import *
from RPS import *
from Animation import *

x = 100
y = 100
grid = Grid2d(x, y,1,1)
rps = RockPaperScissors(grid,0.8,0.4)


bounds = (x,y)
update_method = AnimationUpdateMethod(rps.getCell,rps.update)
animate(bounds,update_method)