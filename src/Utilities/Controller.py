from Integrator import *

class Controller:
    def __init__(self,integrator,periodicWork=[],statStep=1):
        self.integrator = integrator
        self.periodicWork = periodicWork
        self.statStep = statStep # use this to override how frequently we print to the screen
    def Step(self,nsteps=1):
        for i in range(nsteps):
            self.integrator.Step()
            if self.integrator.Cycle() % self.statStep == 0:
                print("Cycle: %04d"%self.integrator.Cycle(),
                    " Time: %03.3e"%self.integrator.Time(),
                    " dt: %03.3e"%self.integrator.dt)
            if len(self.periodicWork) > 0:
                for work in self.periodicWork:
                    if self.integrator.Cycle() % work.cycle == 0:
                        work()