TMRpcm tmrpcm;
File file;

void sdInit() {
  //tmrpcm.speakerPin = 5;
  SD.begin(SD_PIN);
}



void writeLog(String logDate, String logUserId, String logMessage) {
  file = SD.open("log.csv", FILE_WRITE);
  if (file) {
    file.println(logDate + ";" + logUserId + ";" + logMessage);
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
      return line.substring(0, line.length() - 1);
      break;
    }
  }
  file.close();
}

void playSound() {
  //digitalWrite(11,LOW);
  tmrpcm.play("1.wav");
  delay(1000);
  //digitalWrite(11,LOW);
}
