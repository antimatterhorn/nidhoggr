class Microphone:
    def __init__(self, nodeList, grid,i,j,filename,workCycle=1):
        self.nodeList = nodeList
        self.cycle = workCycle
        self.grid = grid
        self.i = i
        self.j = j
        self.phi = self.nodeList.getFieldDouble("phi")
        self.filename = filename
    def __call__(self,cycle,time,dt):
        phi = self.nodeList.getFieldDouble("phi")
        value = phi[self.grid.index(self.i,self.j,0)]
        with open(self.filename, 'a') as f:
            f.write(f"{time},{value}\n")