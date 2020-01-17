
#include <EEPROM.h>
#include <iarduino_RTC.h>
#include <Nextion.h>
#include <Stepper.h>
#include <SD.h>
#include <SPI.h>
#include <TMRpcm.h>
#include <Servo.h>


//int enteredPills1; // Количество таблеток при добавлении таймера
//int enteredPills2;

char* pageId = "0";
unsigned long lastTime, waitTime, buzzerLastTime, timerStartTime;

Nextion Display(Serial2, 9600);
iarduino_RTC time(RTC_DS1307);
Stepper stepper1(64, 10, 9, 8, 11);
Stepper stepper2(64, 10, 9, 8, 11);
#define BUZZER_PIN 5
#define SD_PIN 4
#define MODE_SWITCH 2
#define OFF_BUTTON 3
#define SERVO_PIN 6
#define SERVO_SWITCH_PIN 7
//------------------------------------------------------------------
#define timerOffset 15000 //В милисекундах
//------------------------------------------------------------------

void(* reset) (void) = 0; // Перезагрузка MK

Servo tableServo;

void setup() {
  pinMode(SERVO_SWITCH_PIN, OUTPUT);
  tableServo.attach(SERVO_PIN);
  Serial.begin(19200);
  stepper1.setSpeed(50);
  stepper2.setSpeed(50);
  goToPage("0"); //инициализируем дисплей, а точнее первую страницу
  time.begin();
  sdInit();
  //playSound();
}

void notificationWindow(char* ititle, char* itext, char* ibuttonText = "Ок") {//nextion
  goToPage("7");
  setText("t0", ititle);
  setText("t1", itext);
  setText("b0", ibuttonText);
  while (Display.listen() != "65 7 2 1 ffff ffff ffff");
}

void loop() {

  if (digitalRead(MODE_SWITCH)) {
    displayTime();
    for (int n = 1; n <= EEPROM.read(0); n++) {
      if (EEPROM.read(cellFormula(n, 1)) == time.Hours && EEPROM.read(cellFormula(n, 2)) == time.minutes && millis() - lastTime >= waitTime) { //формула обращения к ячейке n*7-p+3
        lastTime = millis();
        waitTime = 59800;
        goToPage("7");
        bool sdInit = SD.begin(SD_PIN);
        if (sdInit) {
        char nameChar[15];
        String nameStr = getName(EEPROM.read(cellFormula(n, 5)) - 1);
        nameStr.toCharArray(nameChar, 16);
        setText("t0", nameChar);}
        else{
          setText("t0", "Человек " + char(EEPROM.read(cellFormula(n, 5))));
          }
        setText("t1", "Возьмите пожалуйста таблетки!");
        setText("b0", "Взял");
        timerStartTime = millis();
        while (Display.listen() != "65 7 2 1 ffff ffff ffff" && digitalRead(OFF_BUTTON) == 0) {
          if ((millis() % 1000) > 500) {
            tone(BUZZER_PIN, 700);
          }
          else {
            noTone(BUZZER_PIN);
          }
          if(timerStartTime + timerOffset < millis()){
            tone(BUZZER_PIN, 200);
            setText("t0", "Внимание!");
            setText("t1", "Производится утилизация! Уберите руки!");
            setText("b0", "");
            utilizate();
            break;
          }
        }
        noTone(BUZZER_PIN);
        if (sdInit) {
        writeLog(time.gettime("d.m.Y;H:i"), String(EEPROM.read(cellFormula(n, 5))), "\xd2\xe0\xe1\xeb\xe5\xf2\xea\xe0 \xc2\xe7\xff\xf2\xe0");}
      }
    }
  }
  else {
    if (pageId != "1") {
      goToPage("1");
    }
    String nexMessage = Display.listen();
    if (nexMessage != "") {


      if (nexMessage == "65 1 4 1 ffff ffff ffff") {
        time.settime(0, numInput("Введите минуты:", 0, 59), numInput("Введите часы:", 0, 23), numInput("Введите день:", 1, 31), numInput("Введите месяц:", 1, 12), numInput("Введите год:", 0, 99), weekDayInput());
        delay(100);
        reset();
      }
      if (nexMessage == "65 1 2 1 ffff ffff ffff") {
        timerDelete();
      }
      if (nexMessage == "65 1 1 1 ffff ffff ffff") {
        int numberOfTimer = EEPROM.read(0) + 1;
        EEPROM.update(0, numberOfTimer);
        delay(10);
        EEPROM.update(cellFormula(numberOfTimer, 1), numInput("Введите часы:", 0, 23));
        delay(10);
        EEPROM.update(cellFormula(numberOfTimer, 2), numInput("Введите минуты:", 0, 59));
        delay(10);
        EEPROM.update(cellFormula(numberOfTimer, 3), numInput("Кол-во таблеток в I:", 0, 15));
        delay(10);
        EEPROM.update(cellFormula(numberOfTimer, 4), numInput("Кол-во таблеток в II:", 0, 15));
        delay(10);
        EEPROM.update(cellFormula(numberOfTimer, 5), numInput("ID пользователя", 1, 9));
        delay(10);
        reset();
      }

    }
    delay(100);
  }
}



int cellFormula(int timerNumber, int timerParametr) {
  return (10 + timerNumber * 7 - (7 - timerParametr));
}

void utilizate(){
  digitalWrite(SERVO_SWITCH_PIN, HIGH);
  tableServo.write(50);
  delay(1000);
  tableServo.write(5);
  delay(1000);
  digitalWrite(SERVO_SWITCH_PIN, LOW);
  }

void drumRotate(int drum, int numberCells, int reverse = 1) {
  for (int c; c <= numberCells; c++) {
    if (drum) {
      stepper1.step(141 * reverse);
      delay(1000);
    }
    else {
      stepper2.step(141 * reverse);
      delay(1000);
    }
  }
}
