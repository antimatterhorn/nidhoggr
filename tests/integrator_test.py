from nidhoggr import *

class dumpState:
    def __init__(self,nodeList,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.dump = []
    def __call__(self,cycle,time,dt):
        y = self.nodeList.getFieldDouble("y")[0]
        self.dump.append([time,y])

if __name__ == "__main__":
    commandLine = CommandLineArguments(cycles=200)
    
    myNodeListEul = NodeList(1)
    myNodeListRK2 = NodeList(1)
    myNodeListRK4 = NodeList(1)
    myNodeListCN  = NodeList(1)

    dtmin = 1.0/cycles

    #constants = PhysicalConstants(1.0, 1.0, 1.0)
    constants = MKS()
    
    import time

    # ---------------------------
    # Euler
    # ---------------------------
    physics = ImplicitPhysics2d(myNodeListEul, constants)
    packages = [physics]
    integrator = Integrator2d(packages=packages, dtmin=dtmin, verbose=False)
    dumpEul = dumpState(myNodeListEul, workCycle=1)
    controller = Controller(integrator=integrator, periodicWork=[dumpEul], statStep=cycles*2, tstop=1)

    start = time.time()
    controller.Step(cycles)
    elapsed = time.time() - start
    print(f"Euler: {controller.cycle} cycles in {elapsed:.6f} seconds")

    # ---------------------------
    # RK2
    # ---------------------------
    physics = ImplicitPhysics2d(myNodeListRK2, constants)
    packages = [physics]
    integrator = RungeKutta2Integrator2d(packages=packages, dtmin=dtmin, verbose=False)
    dumpRK2 = dumpState(myNodeListRK2, workCycle=1)
    controller = Controller(integrator=integrator, periodicWork=[dumpRK2], statStep=cycles*2, tstop=1)

    start = time.time()
    controller.Step(cycles)
    elapsed = time.time() - start
    print(f"RK2: {controller.cycle} cycles in {elapsed:.6f} seconds")

    # ---------------------------
    # RK4
    # ---------------------------
    physics = ImplicitPhysics2d(myNodeListRK4, constants)
    packages = [physics]
    integrator = RungeKutta4Integrator2d(packages=packages, dtmin=dtmin, verbose=False)
    dumpRK4 = dumpState(myNodeListRK4, workCycle=1)
    controller = Controller(integrator=integrator, periodicWork=[dumpRK4], statStep=cycles*2, tstop=1)

    start = time.time()
    controller.Step(cycles)
    elapsed = time.time() - start
    print(f"RK4: {controller.cycle} cycles in {elapsed:.6f} seconds")

    # ---------------------------
    # Crank-Nicolson
    # ---------------------------
    physics = ImplicitPhysics2d(myNodeListCN, constants)
    packages = [physics]
    integrator = CrankNicolsonIntegrator2d(packages=packages, dtmin=dtmin, verbose=False)
    dumpCN = dumpState(myNodeListCN, workCycle=1)
    controller = Controller(integrator=integrator, periodicWork=[dumpCN], statStep=cycles*2, tstop=1)

    start = time.time()
    controller.Step(cycles)
    elapsed = time.time() - start
    print(f"Crank–Nicolson: {controller.cycle} cycles in {elapsed:.6f} seconds")

    
    import matplotlib.pyplot as plt
    import numpy as np

    def theta(t):
        return -t*t - 2*t + 2*np.exp(t) - 2

    theta_vec = np.vectorize(theta)
    t_values = np.linspace(0, cycles*dtmin, cycles*10)

    xe,ye       = zip(*dumpEul.dump)
    xr2,yr2     = zip(*dumpRK2.dump)
    xr4,yr4     = zip(*dumpRK4.dump)
    xrc,yrc     = zip(*dumpCN.dump)

    xs = t_values
    ys = theta_vec(t_values)

    plt.plot(xs, ys, '-', label="analytic", linewidth=2, color='black')
    plt.plot(xe, ye, 'o', label="Euler Step", markersize=4)
    plt.plot(xr2, yr2, 's', label="RK2", markersize=4)
    plt.plot(xr4, yr4, '^', label="RK4", markersize=4)
    plt.plot(xrc, yrc, 'd', label="CN", markersize=4)

    plt.xlabel('t')
    plt.ylabel('y')

    plt.grid(True)
    plt.legend(loc='best')
    plt.show()
    
    # Compute analytic values at integrator time samples
    ye_analytic  = theta_vec(np.array(xe))
    yr2_analytic = theta_vec(np.array(xr2))
    yr4_analytic = theta_vec(np.array(xr4))
    yrc_analytic = theta_vec(np.array(xrc))

    # Compute residuals
    residual_euler = abs(np.array(ye)  - ye_analytic)
    residual_rk2   = abs(np.array(yr2) - yr2_analytic)
    residual_rk4   = abs(np.array(yr4) - yr4_analytic)
    residual_cn    = abs(np.array(yrc) - yrc_analytic)

    # Plot residuals
    plt.figure(figsize=(10, 6))

    plt.plot(xe,  residual_euler, 'o-', label="Euler Residual")
    plt.plot(xr2, residual_rk2,   's-', label="RK2 Residual")
    plt.plot(xr4, residual_rk4,   '^-', label="RK4 Residual")
    plt.plot(xrc, residual_cn,    'd-', label="CN Residual")

    plt.axhline(0.0, color='gray', linestyle='--', linewidth=1)

    plt.xlabel("t")
    plt.ylabel("Residual (numerical - analytic)")
    plt.yscale("log") 
    plt.title("Integrator Residuals")
    plt.grid(True)
    plt.legend(loc="best")
    plt.tight_layout()
    plt.show()