import sys, csv, os
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
from gui.logPage import *


class MyWin(QtWidgets.QMainWindow):

    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)

        self.ui = Ui_logWindow()
        self.ui.setupUi(self)
        self.ui.openButton.clicked.connect(self.openLogFile)
        self.ui.clearButton.clicked.connect(self.clearLogFile)
        self.tableFillFromCsv("log.csv", ["Первый", "Второй", "Третий"])

    def clearLogFile(self):
        with open("log.csv", "w") as file:
            file.truncate()
        self.tableFillFromCsv("log.csv", ["Первый", "Второй", "Третий"])

    def openLogFile(self):
        os.popen("log.csv")

    def tableFillFromCsv(self, csvPath, headers = []):
        table = []
        try:
            with open(csvPath, newline='') as file:
                for row in csv.reader(file, delimiter=";"):
                    table.append(row)
            table[0]  # проверка на пустой csv
            print(1)
            self.ui.table.setColumnCount(len(table[0]))
            self.ui.table.setRowCount(len(table))
            self.ui.table.setHorizontalHeaderLabels(headers)
            for row in range(len(table)):
                for col in range(len(table[0])):
                    self.ui.table.setItem(row, col, QTableWidgetItem(table[row][col]))
        except IndexError:
            print(2)
            self.ui.table.setRowCount(0)
            self.ui.table.setColumnCount(0)


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    myapp = MyWin()
    myapp.show()
    sys.exit(app.exec_())