# nodes/base_node.py
from PyQt5.QtWidgets import QGraphicsItem, QGraphicsTextItem
from PyQt5.QtGui import QPainter, QPen, QBrush
from PyQt5.QtCore import QRectF, QPointF, Qt

PORT_RADIUS = 5

class BaseNode(QGraphicsItem):
    def __init__(self, title="Node", inputs=None, outputs=None):
        super().__init__()
        self.title = title
        self.inputs = inputs or []
        self.outputs = outputs or []
        self.titleItem = QGraphicsTextItem(self.title, self)

        self.setFlags(QGraphicsItem.ItemIsMovable | QGraphicsItem.ItemIsSelectable)
        self.width = 150
        self.height = max(60, 20 * max(len(self.inputs), len(self.outputs)))

    def boundingRect(self):
        return QRectF(0, 0, self.width, self.height)

    def paint(self, painter, option, widget):
        painter.setPen(QPen(Qt.black))
        painter.setBrush(QBrush(Qt.lightGray if not self.isSelected() else Qt.yellow))
        painter.drawRect(self.boundingRect())

        # Draw title
        self.titleItem.setPos(10, 0)

        # Draw input ports
        for i, label in enumerate(self.inputs):
            y = 30 + i * 20
            painter.setBrush(QBrush(Qt.blue))
            painter.drawEllipse(QPointF(0, y), PORT_RADIUS, PORT_RADIUS)
            painter.drawText(10, y + 4, label)

        # Draw output ports
        for i, label in enumerate(self.outputs):
            y = 30 + i * 20
            painter.setBrush(QBrush(Qt.green))
            painter.drawEllipse(QPointF(self.width, y), PORT_RADIUS, PORT_RADIUS)
            painter.drawText(self.width - 60, y + 4, label)
