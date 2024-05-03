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
rps = RockPaperScissors(grid=grid,A=0.8,D=0.4)


bounds = (x,y)
update_method = AnimationUpdateMethod(call=rps.getCell,stepper=rps.update)
animate(bounds,update_method)
