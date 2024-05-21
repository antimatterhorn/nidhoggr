from nidhoggr import *

if __name__ == "__main__":
    travelTime = 2 # years
    distance   = 4  # light-years

    td = TimeDilation(travelTime,distance)

    print("To travel %3.2f light-years in %3.2f years, \nyou would need to travel %3.3f c \nand %3.3f years would pass on Earth"%(distance,travelTime,td.velocity,td.earthTime))