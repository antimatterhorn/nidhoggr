print("this package requires that you've installed PIL already")

from PIL import Image,ImageOps # requires pip3 install PIL

class PyImage:
    def __init__(self,fileName):
        self.img = Image.open(fileName)
        return