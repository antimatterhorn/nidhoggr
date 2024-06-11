from nidhoggr import *
from Animation import *

if __name__ == "__main__":
    animate = True
    
    numNodes = 100

    constants = MKS()
    myNodeList = NodeList(numNodes)
    gravVec = Vector2d(0,-9.8)

    constantGravity = ConstantGravity2d(myNodeList,constants,gravVec)
    packages = [constantGravity]

    pm = SphereParticleBoundary2d(physics=constantGravity,position=Vector2d(0.5,0),radius=2)
    pbounds = [pm]

    integrator = RungeKutta2Integrator2d(packages=packages,
                              dtmin=0.01,
                              boundaries=pbounds)
    print(integrator)

    myNodeList.insertFieldDouble("radius")
    rad = myNodeList.getFieldDouble("radius")
    for i in range(numNodes):
        rad.setValue(i,0.1)

    print("numNodes =",myNodeList.numNodes)
    print("field names =",myNodeList.fieldNames)

    pos = myNodeList.getFieldVector2d("position")
    import random
    for i in range(numNodes):
        pos.setValue(i,Vector2d(random.uniform(-10, 10),10))

    controller = Controller(integrator=integrator,periodicWork=[],statStep=100)

    if(animate):
        title = MakeTitle(controller,"time","time")

        bounds = (-10,10,-10,10)
        update_method = AnimationUpdateScatterMethod2d(posField=pos,
                                                stepper=controller.Step,
                                                title=title)
        AnimateScatter(bounds,update_method,frames=10)
    else:
        controller.Step(5000)
