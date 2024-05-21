from nidhoggr import *

if __name__ == "__main__":
    travelTime = 4 # years
    distance   = 4  # light-years

    td = TimeDilation(travelTime,distance)
    td.compute()

    print("you would need to travel %3.3f c and %3.3f years would pass on Earth"%(td.velocity,td.earthTime))