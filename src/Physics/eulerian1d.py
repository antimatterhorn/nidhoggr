from math import sqrt
from re import L
from matplotlib import pyplot as plt
import numpy as np
from copy import deepcopy

# somes notes on F(U)
# f[0] = u[1]
# f[1] = u[1]*u[1]/u[0] + Pr
# f[2] = u[1](u[2]+1)/u[0]

efloor      = 1e-10
rhofloor    = 1e-33
cfl         = 0.4
gam         = 1.67 # adiabatic index (6.1 for water)

p0          = 0.0 # 2.0e10
rho0        = 1e-3

nzones = 500
xsize = 100.
dx = xsize/nzones
tpos = 0
dt = 1e-9

maxiter = 2000
zones = []

class zone:
    def __init__(self,x=0,Pr=0,rho=0,e=0,v=0,cs=0):
        self.x = x
        self.u = [0,0,0] # state vector
        self.f = [0,0,0] # eigenvector
        self.Pr = Pr
        self.rho = rho
        self.e = e
        self.v = v
        self.cs = cs

        self.ep = 0
        self.em = 0
        self.etot = 0
        self.htot = 0 # total specific enthalpy

    def __repr__(self):
        return '(x=' + str(self.x) + ', rho=' + str(self.rho) + ', v=' + str(self.v) + ')'

    def copyState(self,other):
        self.Pr = other.Pr
        self.rho = other.rho
        self.e = other.e
        self.v = other.v
        self.cs = other.cs
        self.ep = other.ep
        self.em = other.em
        for i in range(3):
            self.u[i] = other.u[i]
            self.f[i] = other.f[i]

# initial setup
for i in range(nzones):
    x = float(i)*dx
    rho = rho0
    #v = 0 if x > 10.0 else 1.0 # sedov IC
    v = 0
    Pr = 1.0e6 if (x<45 or x>55) else 2.0e6 # 1atm
    e = (Pr+gam*p0)/((gam-1)*rho)
    cs = sqrt(gam*(Pr+p0)/rho)
    thisZone = zone(x,Pr,rho,e,v,cs)
    thisZone.etot = e + 0.5*v**2
    thisZone.htot = thisZone.etot + Pr/rho

    thisZone.u[0] = rho
    thisZone.u[1] = rho*v
    thisZone.u[2] = rho*thisZone.etot
    u = thisZone.u

    thisZone.f[0] = u[1]
    thisZone.f[1] = u[1]*u[1]/u[0] + Pr
    thisZone.f[2] = u[1]*thisZone.htot

    thisZone.ep = v + cs
    thisZone.em = v - cs

    zones.append(thisZone)

#zones[nzones-2].copyState(zones[0])
#zones[1].copyState(zones[nzones-1])

def applyBoundaries():
    zones[0].copyState(zones[1])
    zones[nzones-1].copyState(zones[nzones-2])

applyBoundaries()

# advection vars
uL = np.zeros(3)
uR = np.zeros(3)
fL = np.zeros(3)
fR = np.zeros(3)
flp = np.zeros(3)
flm = np.zeros(3)
Lv = np.zeros(3)


# plotting
def plot(idx=0,var="rho",save=True):
    xs = np.zeros(nzones)
    ys = np.zeros(nzones)
    for i in range(nzones):
        xs[i] = zones[i].x
        if var=="rho":
            ys[i] = zones[i].rho
        elif var=="Pr":
            ys[i] = zones[i].Pr
        elif var=="v":
            ys[i] = zones[i].v
        elif var=="e":
            ys[i] = zones[i].e
    #plt.ylim((0.999e6,1.001e6))
    plt.plot(xs,ys)
    #plt.show()
    if save:
        plt.savefig("%s.png"%idx)
    else:
        plt.show()
    plt.clf()

plot(0)

iter = 0
while (iter < maxiter):
    iter+=1
    tpos += dt

    uZones = deepcopy(zones)
    for i in range(1,nzones-1):
        # solve Fi+1/2
        # so L = i and R = i-1
        epR = 0.5*(zones[i+1].ep + zones[i].ep)
        epL = zones[i].ep
        emR = 0.5*(zones[i+1].em + zones[i].em)
        emL = zones[i].em

        ap = max(epL,epR,0)
        am = max(-emL,-emR,0)

        for j in range(3):
            fL[j] = zones[i].f[j]
            fR[j] = 0.5*(zones[i+1].f[j] + fL[j])
            uL[j] = zones[i].u[j]
            uR[j] = 0.5*(zones[i+1].u[j] + uL[j])
            flp[j] = (ap*fL[j] + am*fR[j] - ap*am*(uR[j]-uL[j]))/(ap+am)
        # solve Fi-1/2
        # so L = i-1 and R = i
        epR = epL
        emR = emL
        epL = 0.5*(zones[i-1].ep + epR)
        emL = 0.5*(zones[i-1].em + emR)

        ap = max(epL,epR,0)
        am = max(-emL,-emR,0)

        for j in range(3):
            fR[j] = fL[j]
            uR[j] = uL[j]
            fL[j] = 0.5*(zones[i-1].f[j] + fR[j])
            uL[j] = 0.5*(zones[i-1].u[j] + uR[j])
            flm[j] = (ap*fL[j] + am*fR[j] - ap*am*(uR[j]-uL[j]))/(ap+am)
            Lv[j] = (flm[j] - flp[j])
            uZones[i].u[j] += dt*Lv[j]/dx

    me = 0 # max eigenval
    for i in range(nzones):
        zones[i].copyState(uZones[i])

        # update quantities
        rho    = max(zones[i].u[0],rhofloor)
        v      = zones[i].u[1]/rho
        etot   = max(efloor,zones[i].u[2]/rho)
        e      = etot - 0.5*v**2
        zones[i].rho    = rho
        zones[i].v      = v
        zones[i].etot   = etot
        zones[i].e      = e

        # now back to state vector
        zones[i].u[0]   = rho
        zones[i].u[1]   = rho*v
        zones[i].u[2]   = rho*etot

        # eos
        zones[i].Pr     = (gam-1.0)*rho*e-gam*p0
        zones[i].cs     = sqrt(gam*(zones[i].Pr+p0)/rho)
        zones[i].etot   = zones[i].e + 0.5*zones[i].v**2
        zones[i].htot   = etot + zones[i].Pr/rho

        # update eigenvals
        zones[i].ep     = v + zones[i].cs
        zones[i].em     = v - zones[i].cs

        # update eigenvector
        zones[i].f[0]   = zones[i].u[1]
        zones[i].f[1]   = zones[i].u[1]*zones[i].u[1]/zones[i].u[0]+zones[i].Pr
        zones[i].f[2]   = zones[i].u[1]*zones[i].htot

        me = max(me,abs(zones[i].ep),abs(zones[i].em))
    
    applyBoundaries()

    if (iter % 10 == 0):
        print("ITER %04d\ntime = %3.2e dt = %3.2e\n"%(iter,tpos,dt))
        plot(idx=iter)
    
    dt = cfl*dx/me

