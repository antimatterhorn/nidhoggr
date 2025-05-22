from PyQt5.QtWidgets import QGraphicsScene, QMenu, QAction
from PyQt5.QtCore import Qt
from nodes import node_registry

class GraphScene(QGraphicsScene):
    def __init__(self, parent=None):
        super().__init__(parent)
        self.setSceneRect(-500, -500, 1000, 1000)

    def contextMenuEvent(self, event):
        menu = QMenu()
        for name in node_registry:
            action = QAction(name, menu)
            action.triggered.connect(lambda checked=False, name=name, pos=event.scenePos(): self.addNode(name, pos))
            menu.addAction(action)
        menu.exec_(event.screenPos())

    def addNode(self, name, pos):
        if name in node_registry:
            node = node_registry[name]()
            node.setPos(pos)
            self.addItem(node)
