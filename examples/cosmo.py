from nidhoggr import *

if __name__ == "__main__":
    z = 10
    omega_m = 0.3
    lam = 0.7
    h0 = 70

    cosmo = Cosmology(z=z,omega_m=omega_m,lam=lam,h0=h0)