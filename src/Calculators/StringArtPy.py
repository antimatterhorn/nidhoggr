import matplotlib.pyplot as plt
import numpy as np
from math import *
from random import randrange

from nidhoggr import StringArt
from PyImage import PyImage
from PIL import Image,ImageOps

class StringArtPy:
    def __init__(self,fileName,nails=300,windings=1000,mode="black",width=0.2):
        assert mode in ("black","white")
        if mode == "black":
            m = 0
            bg = "white"
        elif mode == "white":
            m = 1
            bg = "black"
        #self.sa = StringArt()

        img = PyImage(fileName).img
        width,height = img.size
        height = img.height
        if width > height:
            diff = width-height
            left = diff//2
            right = left+height
            top = 0
            bottom = height
            img = img.crop((left,top,right,bottom))
        elif height > width:
            diff = height - width
            left = 0
            right = width
            top = diff//2
            bottom = top+width
            img = img.crop((left,top,right,bottom))

        img = ImageOps.grayscale(img)

        img.show()
        img = img.transpose(Image.FLIP_TOP_BOTTOM)
        self.pix = img.load()

        # assume width and height are the same because it's just simpler if they are
        # therefore, your image had better be square...
        for i in range(img.width):
            x = i - img.width//2
            for j in range(img.width):
                y = j - img.width//2
                r = sqrt(x*x+y*y)
                if r > img.width//2:
                    self.pix[i,j] = 255*(1-m)

        self.xsize  = img.width-1
        self.ysize  = self.xsize
        self.radius = self.xsize//2
        return 