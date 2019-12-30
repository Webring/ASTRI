#include <SD.h>
#include <SPI.h>

void sdInit() {
  if (!SD.begin(4)) {
    Serial.print("The SD card cannot be found");
    while (1);
  }
}

File file;

void writeLog(String logData, String logUserId, String logMessage) {
  file = SD.open("log.csv", FILE_WRITE);
  if (file) {
    file.println(logData + ";" + logUserId + ";" + logMessage);
    file.close(); 
  }
  file.close();
}

String getName(int id) { //получение имен с sd в файле names.txt
  String line;
  file = SD.open("names.txt");
  if (!file) {
    Serial.print("The text file cannot be opened");
    while (1);
  }

  for (int num = 0; file.available(); num++) {
    line = file.readStringUntil('\n');
    if (id == num) {
      if (num == 0) {
        return line.substring(3, line.length() - 1);
      }
      else {
        return line.substring(0, line.length() - 1);
      }
      break;
    }
  }
  file.close();
}
