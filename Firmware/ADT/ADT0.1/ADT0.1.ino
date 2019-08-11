#include <EEPROM.h>
#include <iarduino_RTC.h>
#include <Nextion.h>

char* pageId = "0";
unsigned long lt,vt;

Nextion Display(Serial2, 9600);
iarduino_RTC time(RTC_DS1307);
void(* reset) (void) = 0;

void setup() {
  Serial.begin(9600);
  //sdInit();
  goToPage("0"); //инициализируем дисплей, а точнее первую страницу
  time.begin();
  //writeLog();
  EEPROM.update(0, 1);
  EEPROM.update(3, 12);
  EEPROM.update(4, 30);

}

void loop() {
if(digitalRead(2)){ 
displayTime();
for(int n=1; n<=EEPROM.read(0);n++){
    if(EEPROM.read(n*7-4)==time.hours&&EEPROM.read(n*7-3)==time.minutes&&millis()-lt>=vt){//формула обращения к ячейке n*7-p+3
      lt = millis();
      vt = 59800;
      Serial.println("Кек");
      }
  }
}
else{
  if(pageId != "1"){goToPage("1");}
  if (Display.listen() == "65 1 1 1 ffff ffff ffff") {
    int n = EEPROM.read(0)+1;
    int addTimer[] = {};
    
    }
  if (Display.listen() == "65 1 4 1 ffff ffff ffff") {
    time.settime(0,numInput("Введите минуты:", 0,59),numInput("Введите часы:", 0,23),numInput("Введите день:", 1,31),numInput("Введите месяц:", 1,12),numInput("Введите год:", 0,99),weekDayInput());
    delay(100);
    reset();
    }
  }
}
