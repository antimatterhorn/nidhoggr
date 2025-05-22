from PyQt5.QtWidgets import QApplication, QMainWindow
from graph_view import GraphView
import sys

class MainWindow(QMainWindow):
    def __init__(self):
        super().__init__()
        self.setWindowTitle("Nidhoggr Node Editor")
        self.setGeometry(100, 100, 1200, 800)
        self.graphView = GraphView()
        self.setCentralWidget(self.graphView)

if __name__ == "__main__":
    app = QApplication(sys.argv)
    window = MainWindow()
    window.show()
    sys.exit(app.exec_())
