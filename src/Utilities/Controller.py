from Integrator import *

class Controller:
    def __init__(self,integrator,dumpStep=None):
        self.integrator = integrator
        self.dumpStep = dumpStep
    def Step(self,nsteps=1):
        for i in range(nsteps):
            self.integrator.Step()
            print("Cycle: %04d"%self.integrator.Cycle(),"\tTime: %03.3e"%self.integrator.Time())