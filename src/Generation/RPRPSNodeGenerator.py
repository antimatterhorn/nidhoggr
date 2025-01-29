from LinearAlgebra import *
from math import cos,sin,acos,asin,sqrt
from numpy import pi

class ParameterizedSpiralSurface3d:
    # generates a surface of particles on a unit sphere using PS
    def __init__(self,nshell):
        self.positions = []
        for i in range(1, nshell+1):
            h = -1.0+(2.0*(i-1.0)/(nshell-1.0))
            t = acos(h)
            if(i>1 and i<nshell):
                p = (p+3.8/sqrt(nshell)*1.0/sqrt(1.0-h*h))%(2.0*pi)
            else:
                p = 0
            x = sin(t)*cos(p)
            y = sin(t)*sin(p)
            z = cos(t)

            self.positions.append([x,y,z])
        
        return

class RecursivePrimitiveRefinementSurface3d:
    # generates a surface of particles on a unit sphere using RPR
    def __init__(self,nshell):
        self.positions      = []     # [index,[point]]
        self.middlePoints   = []  # [i,[key,index]]
        self.faces          = []
        self.index          = 0
        counts = []
        nr = 0
        ver = 0
        
        # shapeData = [Nf,Ne,Nc]
        
        shapeData = [[ 6, 9, 5],
                     [ 8,12, 6],
                     [12,18, 8],
                     [20,30,12]]
        
        # first column is total number of shell points
        # second column is number of refinements to reach that shell count
        # third column is shape choice that reaches that shell count
        resolution = [[5,0,0],
                      [6,0,1],
                      [8,0,2],
                      [12,0,3],
                      [14,1,0],
                      [18,1,1],
                      [26,1,2],
                      [42,1,3],
                      [50,2,0],
                      [66,2,1],
                      [98,2,2],
                      [162,2,3],
                      [194,3,0],
                      [258,3,1],
                      [386,3,2],
                      [642,3,3],
                      [770,4,0],
                      [1026,4,1],
                      [1538,4,2],
                      [2562,4,3],
                      [3074,5,0],
                      [4098,5,1],
                      [6146,5,2],
                      [10242,5,3],
                      [12290,6,0],
                      [16386,6,1],
                      [24578,6,2],
                      [40962,6,3],
                      [49154,7,0],
                      [65538,7,1],
                      [98306,7,2],
                      [163842,7,3],
                      [196610,8,0],
                      [262146,8,1],
                      [393218,8,2]]
        
        if (nshell > 4):
            for i in range(len(shapeData)):
                nc  = 0
                nco = 0
                nrf = 0
                while (nc < nshell):
                    nrf += 1
                    nco = nc
                    nc = self.shapeCount(nrf,shapeData[i])
                counts.append([i,nrf-1,nco])
                counts.append([i,nrf,nc])

            diff = 1e13
            for i in range(len(counts)):
                dd = abs(counts[i][2] - nshell)
                if (dd < diff):
                    diff = dd
                    ver = counts[i][0]
                    nr = counts[i][1]

            if (nr<0):
                nr = 0

            if (ver==0):
                self.createHexaSphere(nr)
            elif (ver==1):
                self.createOctaSphere(nr)
            elif (ver==2):
                self.createCubicSphere(nr)
            else:
                self.createIcoSphere(nr)

        elif(nshell==1):
            self.positions.append([0,0,0])
        elif(nshell==2):
            self.positions.append([0,0,1])
            self.positions.append([0,0,-1])
        elif(nshell==3):
            t = sqrt(3)/2.0
            self.positions.append([0,1,0])
            self.positions.append([t,-0.5,0])
            self.positions.append([-t,-0.5,0])
        elif(nshell==4):
            t = sqrt(3.0)/3.0
            self.positions.append([t,t,t])
            self.positions.append([t,-t,-t])
            self.positions.append([-t,-t,t])
            self.positions.append([-t,t,-t])
        
        return

    #---------------------------------------------------------------------------
    # Compute the number of vertices for a given shape at a specific refinement
    # level.
    #  new formula for calculating number of points for a given subdivision level
    #  (Nf * Np(n) - Ne * Npe(n) + Nc)
    #  Nf = Number of faces of primitive shape
    #  Np(n) = Number of points in a triangle subdivided n times
    #       2^(2n-1) + 3*2^(n-1) + 1
    #  Ne = Number of edges of primitive shape
    #  Npe(n) = Number of points along an edge of primitive shape subdivided n times
    #       2^n + 1
    #  Nc = Number of corners
    #---------------------------------------------------------------------------
    def shapeCount(self, refinement, shape):
        Nf  = shape[0]
        Ne  = shape[1]
        Nc  = shape[2]
        n   = refinement
    
        Npe = 2**n + 1
        Np  = 2**(2*n-1) + 3*(2**(n-1)) + 1
        return (Nf * Np - Ne * Npe + Nc)

    #---------------------------------------------------------------------------
    # Mechanics for creating and refining the icosahedron
    #---------------------------------------------------------------------------
    def addVertex(self,point):
        length = sqrt(point[0]*point[0] + point[1]*point[1] + point[2]*point[2])
        self.positions.append([point[0]/length,point[1]/length,point[2]/length])
        self.index = self.index + 1
        return self.index
        
    def checkMiddlePoint(self,key):
        exists  = 0
        myidx   = 0
        for i in range(len(self.middlePoints)):
            if (self.middlePoints[i][0] == key):
                exists = 1
                myidx = self.middlePoints[i][1]
        return exists, myidx
    
    def getMiddlePoint(self,p1,p2):
        firstIsSmaller = (p1<p2)
        smallerIndex = 0
        greaterIndex = 0
        if firstIsSmaller:
            smallerIndex = p1
            greaterIndex = p2
        else:
            smallerIndex = p2
            greaterIndex = p1
        key = smallerIndex * (1e10) + greaterIndex # some giant number
        
        # check if this key already exists in middlepoints
        exists, idx = self.checkMiddlePoint(key)
        if (exists):
            return idx
        
        # otherwise, not already cached, time to add one
        point1 = self.positions[p1]
        point2 = self.positions[p2]
        middle = [(point1[0]+point2[0])/2.0,(point1[1]+point2[1])/2.0,(point1[2]+point2[2])/2.0]
        
        idx = self.addVertex(middle)
        self.middlePoints.append([key,idx-1])
        
        return idx-1
    
    def createIcoSphere(self,np):
        n = 0
        t = (1.0+sqrt(5.0))/2.0
        # create 12 vertices of an icosahedron
        self.addVertex([-1, t, 0])
        self.addVertex([ 1, t, 0])
        self.addVertex([-1,-t, 0])
        self.addVertex([ 1,-t, 0])
        
        self.addVertex([ 0,-1, t])
        self.addVertex([ 0, 1, t])
        self.addVertex([ 0,-1,-t])
        self.addVertex([ 0, 1,-t])
        
        self.addVertex([ t, 0,-1])
        self.addVertex([ t, 0, 1])
        self.addVertex([-t, 0,-1])
        self.addVertex([-t, 0, 1])
        
        # create the 20 initial faces
        # 5 faces around point 0
        self.faces.append([ 0,11, 5])
        self.faces.append([ 0, 5, 1])
        self.faces.append([ 0, 1, 7])
        self.faces.append([ 0, 7,10])
        self.faces.append([ 0,10,11])
        # 5 adjacent faces
        self.faces.append([ 1, 5, 9])
        self.faces.append([ 5,11, 4])
        self.faces.append([11,10, 2])
        self.faces.append([10, 7, 6])
        self.faces.append([ 7, 1, 8])
        # 5 faces around point 3
        self.faces.append([ 3, 9, 4])
        self.faces.append([ 3, 4, 2])
        self.faces.append([ 3, 2, 6])
        self.faces.append([ 3, 6, 8])
        self.faces.append([ 3, 8, 9])
        # 5 adjacent faces
        self.faces.append([ 4, 9, 5])
        self.faces.append([ 2, 4,11])
        self.faces.append([ 6, 2,10])
        self.faces.append([ 8, 6, 7])
        self.faces.append([ 9, 8, 1])
        
        # now refine triangles until you're done
        for i in range(np):
            faces2 = []
            for j in range(len(self.faces)):
                x,y,z = self.faces[j][0], self.faces[j][1], self.faces[j][2]
                a = self.getMiddlePoint(x,y)
                b = self.getMiddlePoint(y,z)
                c = self.getMiddlePoint(z,x)
                
                faces2.append([x,a,c])
                faces2.append([y,b,a])
                faces2.append([z,c,b])
                faces2.append([a,b,c])
            self.faces = faces2
            n = len(self.positions)

    def createOctaSphere(self,np):
        n = 0
        t = sqrt(2.0)/2.0
        # create the 6 vertices of the octahedron
        self.addVertex([ 0, 0, 1])
        self.addVertex([ t, t, 0])
        self.addVertex([ t,-t, 0])
        self.addVertex([-t,-t, 0])
        self.addVertex([-t, t, 0])
        self.addVertex([ 0, 0,-1])
        
        # create the 8 initial faces
        # 4 faces around point 0
        self.faces.append([ 0, 1, 2])
        self.faces.append([ 0, 2, 3])
        self.faces.append([ 0, 3, 4])
        self.faces.append([ 0, 4, 1])
        # 4 faces around point 5
        self.faces.append([ 5, 2, 1])
        self.faces.append([ 5, 3, 2])
        self.faces.append([ 5, 4, 3])
        self.faces.append([ 5, 1, 4])
        
        # now refine triangles until you're done
        for i in range(np):
            faces2 = []
            for j in range(len(self.faces)):
                x,y,z = self.faces[j][0], self.faces[j][1], self.faces[j][2]
                a = self.getMiddlePoint(x,y)
                b = self.getMiddlePoint(y,z)
                c = self.getMiddlePoint(z,x)
                
                faces2.append([x,a,c])
                faces2.append([y,b,a])
                faces2.append([z,c,b])
                faces2.append([a,b,c])
            self.faces = faces2
            n = len(self.positions)

    def createHexaSphere(self,np):
        n = 0
        t = sqrt(3.0)/2.0
        # create the 5 vertices of the hexahedron
        self.addVertex([ 0, 0, 1])
        self.addVertex([ 0, 1, 0])
        self.addVertex([ t,-0.5,0])
        self.addVertex([-t,-0.5,0])
        self.addVertex([ 0, 0,-1])
        
        # create the 6 initial faces
        # 3 faces around point 0
        self.faces.append([ 0, 1, 2])
        self.faces.append([ 0, 2, 3])
        self.faces.append([ 0, 3, 1])
        # 3 faces around point 4
        self.faces.append([ 4, 2, 1])
        self.faces.append([ 4, 3, 2])
        self.faces.append([ 4, 1, 3])
        
        # now refine triangles until you're done
        for i in range(np):
            faces2 = []
            for j in range(len(self.faces)):
                x,y,z = self.faces[j][0], self.faces[j][1], self.faces[j][2]
                a = self.getMiddlePoint(x,y)
                b = self.getMiddlePoint(y,z)
                c = self.getMiddlePoint(z,x)
                
                faces2.append([x,a,c])
                faces2.append([y,b,a])
                faces2.append([z,c,b])
                faces2.append([a,b,c])
            self.faces = faces2
            n = len(self.positions)

    def createCubicSphere(self,np):
        n = 0
        t = sqrt(3.0)/3.0
        # create the 8 vertices of the cube
        self.addVertex([-t, t, t])
        self.addVertex([-t,-t, t])
        self.addVertex([ t,-t, t])
        self.addVertex([ t, t, t])
        self.addVertex([ t, t,-t])
        self.addVertex([ t,-t,-t])
        self.addVertex([-t,-t,-t])
        self.addVertex([-t, t,-t])
        
        # create the 6 initial faces
        # 5 faces around point 0
        self.faces.append([ 0, 4, 7])
        self.faces.append([ 0, 1, 7])
        self.faces.append([ 0, 1, 2])
        self.faces.append([ 0, 2, 3])
        self.faces.append([ 0, 3, 4])
        # 5 faces around point 5
        self.faces.append([ 5, 2, 3])
        self.faces.append([ 5, 3, 4])
        self.faces.append([ 5, 4, 7])
        self.faces.append([ 5, 6, 7])
        self.faces.append([ 5, 2, 6])
        # 2 faces around point 1
        self.faces.append([ 1, 6, 7])
        self.faces.append([ 1, 2, 6])
        
        # now refine triangles until you're done
        for i in range(np):
            faces2 = []
            for j in range(len(self.faces)):
                x,y,z = self.faces[j][0], self.faces[j][1], self.faces[j][2]
                a = self.getMiddlePoint(x,y)
                b = self.getMiddlePoint(y,z)
                c = self.getMiddlePoint(z,x)
                
                faces2.append([x,a,c])
                faces2.append([y,b,a])
                faces2.append([z,c,b])
                faces2.append([a,b,c])
            self.faces = faces2
            n = len(self.positions)

class RPRPSNodeGenerator3d:
    # generates a full 3d distribution of particles using RPR+PS
    def __init__(self,numNodes):
        return

