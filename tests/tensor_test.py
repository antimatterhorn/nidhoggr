from LinearAlgebra import *

if __name__ == "__main__":
    m1 = Tensor2d(1.0,2.0,3.0,4.0)
    m2 = Tensor2d(3.0,-1.0,3.0,2.5)
    
    print("T1 =",m1)
    print("T2 =",m2)
    print("T1+T2 =",m1+m2)
    print("T2*2 = ",m2*2.0)
    print("||T2|| =",m2.determinant)

    m1 = Tensor3d(1.0,2.0,3.0,4.0,5.0,6.0,2.0,1.0,0.0)
    m2 = Tensor3d(3.0,-1.0,3.0,2.5,2.0,3.0,4.0,5.0,6.0)
    
    print("T1 =",m1)
    print("T2 =",m2)
    print("T1+T2 =",m1+m2)
    print("T2*2 = ",m2*2.0)
    print("||T2|| =",m2.determinant)