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
            self.m = 0
            self.bg = "white"
        elif mode == "white":
            self.m = 1
            self.bg = "black"
        self.sa = StringArt()

        img = PyImage(fileName)
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
        pix = img.load()