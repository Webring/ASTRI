import sys, os, shutil, csv
from distutils.dir_util import copy_tree
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import *
from PyQt5.QtWidgets import *
from PyQt5.QtGui import *
#----------настройки------------
SD_CARD_NAME = "ASTRIOS"
BACKUP_DIRECTORY = "fileSystemBackup/"
EXAMPLE_DIRECTORY = "fileSystemExample/"
UPDATE_TIME = 2000
TABLE_HEADERS = ["Дата", "Время", "Пользователь", "Статус"]
#-----------Страницы------------
from gui.mainPage import Ui_mainWindow
from gui.namesPage import Ui_namesWindow
from gui.logPage import Ui_logWindow
#-------------------------------

def sdCardCheck():
    for diskLetter in os.popen("fsutil fsinfo drives").read().replace("\\", "").replace(" ", "").split(":")[1:-1]:
        try:
            if os.popen(f"vol {diskLetter}:").read().split("\n")[0].split()[-1] == SD_CARD_NAME:
                return diskLetter + ":\\"
        except:
            pass
    return 0

class mainWin(QtWidgets.QMainWindow):

    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.ui = Ui_mainWindow()
        self.ui.setupUi(self)

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.tick)
        self.timer.start(UPDATE_TIME)

        #--------------------------------------------
        self.ui.namesButton.clicked.connect(self.showNamesWin)
        self.ui.logButton.clicked.connect(self.showLogWin)
        self.ui.uploadButton.clicked.connect(self.fileSystemClone)
        self.ui.backupButton.clicked.connect(self.fileSystemBackupUpload)
        #--------------------------------------------

    def tick(self):
        if not sdCardCheck():
            self.ui.sdStatus.setText("SD карта не подключена")
            self.ui.sdStatus.setStyleSheet('color: #D40000')
            self.ui.logButton.setEnabled(False)
            self.ui.namesButton.setEnabled(False)
            self.ui.songsButton.setEnabled(False)
            self.ui.checkButton.setEnabled(False)
            self.ui.uploadButton.setEnabled(False)
            self.ui.backupButton.setEnabled(False)
        else:
            self.ui.sdStatus.setText("SD карта подключена")
            self.ui.sdStatus.setStyleSheet('color: #046D05')
            self.ui.logButton.setEnabled(True)
            self.ui.namesButton.setEnabled(True)
            self.ui.songsButton.setEnabled(True)
            self.ui.checkButton.setEnabled(True)
            self.ui.uploadButton.setEnabled(True)
            self.ui.backupButton.setEnabled(True)

    def fileSystemClone(self, disk = "D://", example = EXAMPLE_DIRECTORY):
        disk = sdCardCheck() + "/"
        example = EXAMPLE_DIRECTORY
        copy_tree(disk, BACKUP_DIRECTORY)
        try:
            with os.scandir(disk) as entries:
                for entry in entries:
                    if entry.is_file() or entry.is_symlink():
                        os.remove(entry.path)
                    elif entry.is_dir():
                        shutil.rmtree(entry.path)
        except:
            pass
        copy_tree(example, disk)
        mbox("На SD карту загруженна файловая система. Старые файлы временно сохраненны.", "Сохраненно")

    def fileSystemBackupUpload(self):
        try:
            with os.scandir(sdCardCheck() + "/") as entries:
                for entry in entries:
                    if entry.is_file() or entry.is_symlink():
                        os.remove(entry.path)
                    elif entry.is_dir():
                        shutil.rmtree(entry.path)
        except:
            pass
        copy_tree(BACKUP_DIRECTORY, sdCardCheck() + "/")
        mbox("Файловая система востановленна." , "Сохраненно")

    def showNamesWin(self):
        self.win = namesWin()
        self.win.show()
    def showLogWin(self):
        self.win = logWin()
        self.win.show()


class namesWin(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.ui = Ui_namesWindow()
        self.ui.setupUi(self)
        self.ui.saveButton.clicked.connect(self.writeNames)
        self.uploadNames()

    def uploadNames(self):
        with open(sdCardCheck() + "names.txt", "r", encoding='utf-8-sig') as file:
            names = file.read().split("\n")
        self.ui.name1.setText(names[0])
        self.ui.name2.setText(names[1])
        self.ui.name3.setText(names[2])
        self.ui.name4.setText(names[3])
        self.ui.name5.setText(names[4])
        self.ui.name6.setText(names[5])
        self.ui.name7.setText(names[6])
        self.ui.name8.setText(names[7])
        self.ui.name9.setText(names[8])
        self.ui.name10.setText(names[9])

    def writeNames(self):
        try:
            with open(sdCardCheck() + "names.txt", "w", encoding='utf-8') as file:
                print(self.ui.name1.text(), file=file)
                print(self.ui.name2.text(), file=file)
                print(self.ui.name3.text(), file=file)
                print(self.ui.name4.text(), file=file)
                print(self.ui.name5.text(), file=file)
                print(self.ui.name6.text(), file=file)
                print(self.ui.name7.text(), file=file)
                print(self.ui.name8.text(), file=file)
                print(self.ui.name9.text(), file=file)
                print(self.ui.name10.text(), file=file)
            mbox("Имена были записанны на SD карту", "Сохраненно")
        except:
            pass
        self.hide()

class logWin(QtWidgets.QMainWindow):
    def __init__(self, parent=None):
        QtWidgets.QWidget.__init__(self, parent)
        self.ui = Ui_logWindow()
        self.ui.setupUi(self)

        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.tick)
        self.timer.start(UPDATE_TIME)

        self.ui.openButton.clicked.connect(self.openLogFile)
        self.ui.clearButton.clicked.connect(self.clearLogFile)
        self.tableFillFromCsv(sdCardCheck() + "log.csv", TABLE_HEADERS)

    def tick(self):
        try:
            self.tableFillFromCsv(sdCardCheck() + "log.csv", TABLE_HEADERS)
        except:
            self.hide()

    def clearLogFile(self):
        try:
            with open(sdCardCheck() + "log.csv", "w") as file:
                file.truncate()
            self.tableFillFromCsv(sdCardCheck() + "log.csv", TABLE_HEADERS)
        except:
            self.hide()

    def openLogFile(self):
        try:
            os.popen(sdCardCheck() + "log.csv")
        except:
            self.hide()

    def tableFillFromCsv(self, csvPath, headers=[]):
        table = []
        try:
            with open(csvPath, newline='') as file:
                for row in csv.reader(file, delimiter=";"):
                    table.append(row)
            table[0]  # проверка на пустой csv
            self.ui.table.setColumnCount(len(table[0]))
            self.ui.table.setRowCount(len(table))
            self.ui.table.setHorizontalHeaderLabels(headers)
            for row in range(len(table)):
                for col in range(len(table[0])):
                    self.ui.table.setItem(row, col, QTableWidgetItem(table[row][col]))
            self.ui.table.resizeColumnsToContents()
        except Exception as err:
            print(err)
            self.ui.table.setRowCount(0)
            self.ui.table.setColumnCount(0)


def mbox(body, title="Ошибка!"):
    dialog = QMessageBox(QMessageBox.Information, title, body)
    dialog.exec_()

if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    myapp = mainWin()
    myapp.show()
    sys.exit(app.exec_())

