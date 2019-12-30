String bcode;

void goToPage(char* id) {
  pageId = id;
  Display.init(id);
}

void setText(String element, char* Str) {
  Display.setComponentText(element, convert(Str));// Изменяет текст
  delay(100);
}

int numInput(char* text, int Min, int Max) {
  if (pageId != "2") {
    goToPage("2");
  }
  setText("t0", text);
  setText("t1", "0");
  int numIn[2] = {0, 0};//1 цифра, 2 цифра
  int numNumber = 0;
  int numOut = 0;
  char numSOut[2];
  String numStr;
  while (1) {
    bcode = Display.listen();//обязательно нужна для корректной работы

    if (bcode != "") {
      if (numNumber < 2 && bcode != "65 2 d 1 ffff ffff ffff" && bcode != "65 2 b 1 ffff ffff ffff") {
        if (bcode == "65 2 c 1 ffff ffff ffff") {
          numIn[numNumber] = 0;
        }
        if (bcode == "65 2 1 1 ffff ffff ffff") {
          numIn[numNumber] = 1;
        }
        if (bcode == "65 2 3 1 ffff ffff ffff") {
          numIn[numNumber] = 2;
        }
        if (bcode == "65 2 4 1 ffff ffff ffff") {
          numIn[numNumber] = 3;
        }
        if (bcode == "65 2 5 1 ffff ffff ffff") {
          numIn[numNumber] = 4;
        }
        if (bcode == "65 2 6 1 ffff ffff ffff") {
          numIn[numNumber] = 5;
        }
        if (bcode == "65 2 7 1 ffff ffff ffff") {
          numIn[numNumber] = 6;
        }
        if (bcode == "65 2 8 1 ffff ffff ffff") {
          numIn[numNumber] = 7;
        }
        if (bcode == "65 2 9 1 ffff ffff ffff") {
          numIn[numNumber] = 8;
        }
        if (bcode == "65 2 a 1 ffff ffff ffff") {
          numIn[numNumber] = 9;
        }
        if (bcode != "65 2 c 1 ffff ffff ffff" || numNumber == 1) {
          numNumber++;
        }
      }
      if (numNumber == 2) {
        numOut = numIn[0] * 10 + numIn[1];
      }
      if (numNumber == 1) {
        numOut = numIn[0];
      }
      if (numNumber == 0) {
        numOut = 0;
      }
      if (bcode == "65 2 b 1 ffff ffff ffff") {
        if (numOut <= Max && numOut >= Min) {
          return numOut;
          break;
        }
        else {
          setText("t0", "Неверное значение!");
          delay(1500);
          setText("t0", text);
        }
      }

      if (bcode == "65 2 d 1 ffff ffff ffff") {
        if (numNumber > 0) {
          numNumber--;
        }
        if (numNumber == 0) {
          numOut = 0;
        }
        if (numNumber == 1) {
          numOut = numIn[0];
        }
        if (numNumber == 2) {
          numOut = numIn[0] * 10 + numIn[1];
        }
      }
      numStr = String(numOut);
      numStr.toCharArray(numSOut, 3);
      setText("t1", numSOut);
    }
    delay(100);//задержка для корректной работы дисплея
  }

}
/*
  void drumInput() {
  goToPage("4");
  enteredPills1 = 0;
  enteredPills2 = 0;
  while (1) {
  bcode = Display.listen();
  Serial.println(bcode);
    Serial.println("bc");
      if (bcode == "65 4 1 1 ffff ffff ffff") {
        Serial.println("ok");
        if (enteredPills1 < 15) {
          enteredPills1++;
        } else {
          enteredPills1 = 0;
        } Display.setComponentText("t0", String(enteredPills1));
      }
      if (bcode == "65 4 3 1 ffff ffff ffff") {
        if (enteredPills1 >= 1) {
          enteredPills1--;
        } else {
          enteredPills1 = 15;
        } Display.setComponentText("t0", String(enteredPills1));
      }
      if (bcode == "65 4 5 1 ffff ffff ffff") {
        if (enteredPills2 < 15) {
          enteredPills2++;
        } else {
          enteredPills2 = 0;
        } Display.setComponentText("t1", String(enteredPills2));
      }
      if (bcode == "65 4 6 1 ffff ffff ffff") {
        if (enteredPills2 >= 1) {
          enteredPills2--;
        } else {
          enteredPills2 = 15;
        } Display.setComponentText("t1", String(enteredPills2));
      }
      if (bcode == "65 4 8 1 ffff ffff ffff") {
        break;
      }
      Serial.println("ee");
    delay(10);
  }
  }*/

void infoWindow(char* ititle, char* itext, char* ibuttonText = "Ок") {
  goToPage("7");
  setText("t0", ititle);
  setText("t1", itext);
  setText("b0", ibuttonText);
  while (Display.listen() != "65 7 2 1 ffff ffff ffff") {}
}

int timerDelete() {
  int timerDeleteTimerNumberMax = EEPROM.read(0);
  if (timerDeleteTimerNumberMax == 0) {
    infoWindow("Уведомление!", "У вас нет таймеров!");
  }
  else {
    goToPage("6");
    int timerDeleteTimerNumber = 1;
    Display.setComponentText("t4", String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 1))) + ":" + String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 2))));
    Display.setComponentText("t2", String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 3))) + ";" + String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 4))));
    Display.setComponentText("t5", String(timerDeleteTimerNumber));
    while (1) {
      String bcode = Display.listen();
      if (bcode == "65 6 1 1 ffff ffff ffff") {
        timerDeleteTimerNumber--;
        if (timerDeleteTimerNumber <= 0) {
          timerDeleteTimerNumber = timerDeleteTimerNumberMax;
        }
        Display.setComponentText("t4", String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 1))) + ":" + String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 2))));
        Display.setComponentText("t2", String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 3))) + ";" + String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 4))));
        Display.setComponentText("t5", String(timerDeleteTimerNumber));
      }

      if (bcode == "65 6 2 1 ffff ffff ffff") {
        timerDeleteTimerNumber++;
        if (timerDeleteTimerNumber > timerDeleteTimerNumberMax) {
          timerDeleteTimerNumber = 1;
        }
        Display.setComponentText("t4", String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 1))) + ":" + String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 2))));
        Display.setComponentText("t2", String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 3))) + ";" + String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 4))));
        Display.setComponentText("t5", String(timerDeleteTimerNumber));

      }
      if (bcode == "65 6 3 1 ffff ffff ffff") {
        if (timerDeleteTimerNumber == timerDeleteTimerNumberMax) {
          Serial.println("easy");
          EEPROM.update(0, EEPROM.read(0) - 1);
          for (int par = 1; par <= 7; par++) {
            EEPROM.update(cellFormula(timerDeleteTimerNumber, par), 0);
          }
          timerDeleteTimerNumber--;
          timerDeleteTimerNumberMax = EEPROM.read(0);

        }
        else {
          Serial.println("hard");
          EEPROM.update(0, EEPROM.read(0) - 1);
          for (int par = 1; par <= 7; par++) {
            EEPROM.update(cellFormula(timerDeleteTimerNumber, par), EEPROM.read(cellFormula(timerDeleteTimerNumberMax, par)));
          }
          for (int par = 1; par <= 7; par++) {
            EEPROM.update(cellFormula(timerDeleteTimerNumberMax, par), 0);
          }
          timerDeleteTimerNumberMax = EEPROM.read(0);
        }
        for (int n = 0; n <= 100; n++) {
          Serial.println(EEPROM.read(n));
        }
        Display.setComponentText("t4", String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 1))) + ":" + String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 2))));
        Display.setComponentText("t2", String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 3))) + ";" + String(EEPROM.read(cellFormula(timerDeleteTimerNumber, 4))));
        Display.setComponentText("t5", String(timerDeleteTimerNumber));
      }
      if (bcode == "65 6 9 1 ffff ffff ffff" or timerDeleteTimerNumberMax == 0) {
        break;
      }
      delay(1);
    }
  }
}

int weekDayInput() {
  goToPage("3");
  int in = 1;
  while (1) {
    String bcode = Display.listen();
    if (bcode == "65 3 2 1 ffff ffff ffff") {
      if (in < 6) {
        in++;
      } else {
        in = 0;
      } setText("t0", daysNames(in));
    }
    if (bcode == "65 3 3 1 ffff ffff ffff") {
      return in;
      break;
    }
    if (bcode == "65 3 4 1 ffff ffff ffff") {
      if (in >= 1) {
        in--;
      } else {
        in = 6;
      } setText("t0", daysNames(in));
    }
    delay(1);
  }
}

String convert(const char* in)// функция конвертирования текста перед отправкой данных на дисплей, нужна для корректного отбражения русского языка
{
  String numOut;
  if (in == NULL)
    return numOut;

  uint32_t codepoint = 0;
  while (*in != 0)
  {
    uint8_t ch = (uint8_t) (*in);
    if (ch <= 0x7f)
      codepoint = ch;
    else if (ch <= 0xbf)
      codepoint = (codepoint << 6) | (ch & 0x3f);
    else if (ch <= 0xdf)
      codepoint = ch & 0x1f;
    else if (ch <= 0xef)
      codepoint = ch & 0x0f;
    else
      codepoint = ch & 0x07;
    ++in;
    if (((*in & 0xc0) != 0x80) && (codepoint <= 0x10ffff))
    {
      if (codepoint <= 255)
      {
        numOut += (char) codepoint;
      }
      else
      {
        if (codepoint > 0x400)
          numOut += (char) (codepoint - 0x360);
      }
    }
  }
  return numOut;
}

char* daysNames(int in) {
  char* Day;
  switch (in) {
    case 1:
      Day = "Понедельник";
      break;
    case 2:
      Day = "Вторник";
      break;
    case 3:
      Day = "Среда";
      break;
    case 4:
      Day = "Четверг";
      break;
    case 5:
      Day = "Пятница";
      break;
    case 6:
      Day = "Суббота";
      break;
    case 0:
      Day = "Воскресенье";
      break;
  }
  return Day;
}

void displayTime() {
  if (pageId != "0") {
    goToPage("0");
  }
  setText("main.t0", time.gettime("H:i:s"));
  setText("main.t1", time.gettime("d.m.Y"));
  char* Day;
  setText("t2", daysNames(atoi(time.gettime("w"))));
}
