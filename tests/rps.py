import sys
sys.path.append("../build/src/Utilities")

from nidhoggr import *
from RPS import *
from Animation import *

x = 100
y = 100
grid = Grid2d(x, y,1,1)
print("%i x %i grid"%(x,y))
rps = RockPaperScissors(grid=grid,A=0.8,D=0.4)


bounds = (x,y)
update_method = AnimationUpdateMethod2d(call=rps.getCell,stepper=rps.update)
AnimateGrid2d(bounds,update_method)
