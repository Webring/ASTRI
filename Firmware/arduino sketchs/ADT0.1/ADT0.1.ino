#include <EEPROM.h>
#include <iarduino_RTC.h>
#include <Nextion.h>
#include <Stepper.h>
//int enteredPills1; // Количество таблеток при добавлении таймера
//int enteredPills2;

char* pageId = "0";
unsigned long lastTime, waitTime;

Nextion Display(Serial2, 9600);
iarduino_RTC time(RTC_DS1307);
Stepper stepper1(64, 10, 9, 8, 11);
Stepper stepper2(64, 10, 9, 8, 11);

void(* reset) (void) = 0; // Перезагрузка MK



void setup() {
  Serial.begin(19200);
  //sdInit();
  stepper1.setSpeed(200);
  stepper2.setSpeed(200);
  goToPage("0"); //инициализируем дисплей, а точнее первую страницу
  time.begin();

  /*for (int n = 0; n <= EEPROM.read(0) * 7; n++) {
    Serial.println(EEPROM.read(n));
    }*/
  for (int n = 0; n <= 100; n++) {
    Serial.println(EEPROM.read(n));
  }
  /*for (int par = 0; par < 100; par++) {
          EEPROM.update(par, 0);
        }*/
}

void loop() {

  if (digitalRead(2)) {
    displayTime();
    for (int n = 1; n <= EEPROM.read(0); n++) {
      if (EEPROM.read(cellFormula(n, 1)) == time.Hours && EEPROM.read(cellFormula(n, 2)) == time.minutes && millis() - lastTime >= waitTime) { //формула обращения к ячейке n*7-p+3
        lastTime = millis();
        waitTime = 59800;
        Serial.println(EEPROM.read(cellFormula(n, 3)));
        Serial.println(EEPROM.read(cellFormula(n, 4)));
        //writeLog();
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
        //drumInput();
        //Serial.println(enteredPills1);
        //Serial.println(enteredPills2);
        EEPROM.update(cellFormula(numberOfTimer, 3), numInput("Количество таблеток в I:", 0, 15));
        delay(10);
        EEPROM.update(cellFormula(numberOfTimer, 4), numInput("Количество таблеток в II:", 0, 15));
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

void drumRotate(int drum, int numberCells, int reverse = 1) {
  for (int c; c <= numberCells; c++) {
    if (drum) {
      stepper1.step(128 * reverse);
      delay(1000);
    }
    else {
      stepper2.step(128 * reverse);
      delay(1000);
    }
  }
}
