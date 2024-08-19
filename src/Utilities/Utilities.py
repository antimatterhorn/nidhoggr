from nidhoggr import SiloMeshWriter2d

class Microphone:
    def __init__(self, nodeList, grid,i,j,filename,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.i = i
        self.j = j
        self.phi = self.nodeList.getFieldDouble("phi")
        self.filename = filename
        print("Wave Equation microphone initialized at %d,%d"%(i,j))
    def __call__(self,cycle,time,dt):
        phi = self.nodeList.getFieldDouble("phi")
        value = phi[self.grid.index(self.i,self.j,0)]
        with open(self.filename, 'a') as f:
            f.write(f"{time},{value}\n")

class SiloDump:
    def __init__(self,baseName,nodeList,fieldNames,dumpCycle=10):
        self.meshWriter = SiloMeshWriter2d(baseName="waveBox",nodeList=nodeList,fieldNames=["phi","xi"])
        self.cycle = dumpCycle
    def __call__(self,cycle,time,dt):
        self.meshWriter.write("-cycle=%03d.silo"%(cycle))