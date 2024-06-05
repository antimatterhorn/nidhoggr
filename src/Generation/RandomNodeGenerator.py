from DataBase import *
from LinearAlgebra import *

class RandomNodeGenerator1d:
    def __init__(self, numNodes, bounds=[]):
        self.numNodes = numNodes
        self.bounds = bounds
        self.positions = []
        if len(self.bounds) == 0:
            self.bounds = [0, 1]  # default bounds if none are provided
        import random
        for i in range(numNodes):
            self.positions.append(Vector1d(random.uniform(self.bounds[0], self.bounds[1])))

class RandomNodeGenerator2d:
    def __init__(self, numNodes, bounds=[]):
        self.numNodes = numNodes
        self.bounds = bounds
        self.positions = []
        if len(self.bounds) == 0:
            self.bounds = [[0, 0], [1, 1]]  # default bounds if none are provided
        import random
        for i in range(numNodes):
            x = random.uniform(self.bounds[0][0], self.bounds[1][0])
            y = random.uniform(self.bounds[0][1], self.bounds[1][1])
            self.positions.append(Vector2d(x, y))

class RandomNodeGenerator3d:
    def __init__(self, numNodes, bounds=[]):
        self.numNodes = numNodes
        self.bounds = bounds
        self.positions = []
        if len(self.bounds) == 0:
            self.bounds = [[0, 0, 0], [1, 1, 1]]  # default bounds if none are provided
        import random
        for i in range(numNodes):
            x = random.uniform(self.bounds[0][0], self.bounds[1][0])
            y = random.uniform(self.bounds[0][1], self.bounds[1][1])
            z = random.uniform(self.bounds[0][2], self.bounds[1][2])
            self.positions.append(Vector3d(x, y, z))
