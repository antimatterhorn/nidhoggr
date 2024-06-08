import sys
sys.path.append("../build/src/Utilities")

from nidhoggr import *
from Animation import *

x = 150
y = 150
grid = Grid2d(x, y,1,1)
nodeList = NodeList(x*y)

print("%i x %i grid"%(x,y))
constants = MKS() # this doesn't matter
rps = RockPaperScissors(nodeList=nodeList,
                        constants=constants,
                        grid=grid,A=0.8,D=0.4)

packages = [rps]

integrator = Integrator2d(packages=packages,
                            dtmin=1, # always use 1 for rps since it's stable there
                            boundaries=[])


controller = Controller(integrator=integrator,
                            statStep=10,
                            periodicWork=[])


bounds = (x,y)
update_method = AnimationUpdateMethod2d(call=rps.getCell2d,stepper=controller.Step)
AnimateGrid2d(bounds,update_method,threeColors=True)
