import sys
sys.path.append("../build/src/Utilities")

from nidhoggr import *
from RPS import *
from Animation import *

commandLine = CommandLineArguments(nx = 100,
                                    ny = 100,
                                    A = 0.8,
                                    D = 0.4)


grid = Grid2d(nx, ny,1,1)
print("%i x %i grid"%(nx,ny))
rps = RockPaperScissors(grid=grid,A=A,D=D)


bounds = (nx,ny)
update_method = AnimationUpdateMethod2d(call=rps.getCell,stepper=rps.update)
AnimateGrid2d(bounds,update_method,threeColors=True)
