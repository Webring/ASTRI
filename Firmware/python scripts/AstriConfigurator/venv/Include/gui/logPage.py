# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'logPage.ui'
#
# Created by: PyQt5 UI code generator 5.14.0
#
# WARNING! All changes made in this file will be lost!


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_logWindow(object):
    def setupUi(self, logWindow):
        logWindow.setObjectName("logWindow")
        logWindow.resize(719, 562)
        self.centralwidget = QtWidgets.QWidget(logWindow)
        self.centralwidget.setObjectName("centralwidget")
        self.table = QtWidgets.QTableWidget(self.centralwidget)
        self.table.setGeometry(QtCore.QRect(10, 60, 701, 491))
        font = QtGui.QFont()
        font.setPointSize(15)
        self.table.setFont(font)
        self.table.setEditTriggers(QtWidgets.QAbstractItemView.NoEditTriggers)
        self.table.setObjectName("table")
        self.table.setColumnCount(0)
        self.table.setRowCount(0)
        self.openButton = QtWidgets.QPushButton(self.centralwidget)
        self.openButton.setGeometry(QtCore.QRect(10, 10, 421, 41))
        font = QtGui.QFont()
        font.setPointSize(15)
        self.openButton.setFont(font)
        self.openButton.setObjectName("openButton")
        self.clearButton = QtWidgets.QPushButton(self.centralwidget)
        self.clearButton.setGeometry(QtCore.QRect(450, 10, 261, 41))
        font = QtGui.QFont()
        font.setPointSize(15)
        self.clearButton.setFont(font)
        self.clearButton.setObjectName("clearButton")
        logWindow.setCentralWidget(self.centralwidget)

        self.retranslateUi(logWindow)
        QtCore.QMetaObject.connectSlotsByName(logWindow)

    def retranslateUi(self, logWindow):
        _translate = QtCore.QCoreApplication.translate
        logWindow.setWindowTitle(_translate("logWindow", "Просмотр логов"))
        self.openButton.setText(_translate("logWindow", "Открыть файл"))
        self.clearButton.setText(_translate("logWindow", "Очистить"))


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    logWindow = QtWidgets.QMainWindow()
    ui = Ui_logWindow()
    ui.setupUi(logWindow)
    logWindow.show()
    sys.exit(app.exec_())
