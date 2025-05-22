from PyQt5.QtWidgets import QGraphicsView
from PyQt5.QtGui import QPainter
from graph_scene import GraphScene

class GraphView(QGraphicsView):
    def __init__(self):
        super().__init__()
        self.setRenderHints(QPainter.Antialiasing | QPainter.TextAntialiasing)
        self.scene = GraphScene(self)
        self.setScene(self.scene)
