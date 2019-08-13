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
  int in[2] = {0, 0};//1 цифра, 2 цифра
  int num = 0;
  int out=0;
  char cOut[2];
  String str;
  while (1) {
    String bcode = Display.listen();//обязательно нужна для корректной работы
    
    if(bcode != ""){
    if (num < 2 && bcode!= "65 2 d 1 ffff ffff ffff" && bcode!="65 2 b 1 ffff ffff ffff") {
    if (bcode == "65 2 c 1 ffff ffff ffff") {in[num] = 0;}
    if (bcode == "65 2 1 1 ffff ffff ffff") {in[num] = 1;}
    if (bcode == "65 2 3 1 ffff ffff ffff") {in[num] = 2;}
    if (bcode == "65 2 4 1 ffff ffff ffff") {in[num] = 3;}
    if (bcode == "65 2 5 1 ffff ffff ffff") {in[num] = 4;}
    if (bcode == "65 2 6 1 ffff ffff ffff") {in[num] = 5;}
    if (bcode == "65 2 7 1 ffff ffff ffff") {in[num] = 6;}
    if (bcode == "65 2 8 1 ffff ffff ffff") {in[num] = 7;}
    if (bcode == "65 2 9 1 ffff ffff ffff") {in[num] = 8;}
    if (bcode == "65 2 a 1 ffff ffff ffff") {in[num] = 9;}
          if(bcode != "65 2 c 1 ffff ffff ffff"){num++;}
        }
        if (num == 2) {out = in[0] * 10 + in[1];}
        if(num ==1) {out = in[0];}
        if(num == 0){out=0;}
        if (bcode == "65 2 b 1 ffff ffff ffff") {
      if(out<=Max && out>=Min){Serial.print("out==11 ");Serial.println(out==11);return out;break;}
      else{setText("t0", "Неверное значение!");
      delay(1500);
      setText("t0", text);
      }
      }
    
    if (bcode == "65 2 d 1 ffff ffff ffff") {
      if (num > 0) {num--;}
      if(num ==0){out = 0;}
      if(num == 1){out = in[0];}
      if (num == 2) {out = in[0] * 10 + in[1];}
    }
      str = String(out);
      str.toCharArray(cOut, 3);
      setText("t1", cOut);}
    delay(100);//задержка для корректной работы дисплея
  }
}

int weekDayInput(){
  goToPage("3");
  int in = 1;
  while(1){
  String bcode = Display.listen();
  if(bcode == "65 3 2 1 ffff ffff ffff"){if(in<6){in++;}else{in=0;}setText("t0", daysNames(in));}
  if(bcode == "65 3 3 1 ffff ffff ffff"){return in;break;}
  if(bcode == "65 3 4 1 ffff ffff ffff"){if(in>=1){in--;}else{in=6;}setText("t0", daysNames(in));}
  delay(1);
  }}

String convert(const char* in)// функция конвертирования текста перед отправкой данных на дисплей, нужна для корректного отбражения русского языка
{
  String out;
  if (in == NULL)
    return out;

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
        out += (char) codepoint;
      }
      else
      {
        if (codepoint > 0x400)
          out += (char) (codepoint - 0x360);
      }
    }
  }
  return out;
}

char* daysNames(int in){
  char* Day;
  switch(in){
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
  if(pageId != "0"){goToPage("0");}
  setText("main.t0", time.gettime("H:i:s"));
  setText("main.t1", time.gettime("d.m.Y"));
  char* Day;
  setText("t2", daysNames(atoi(time.gettime("w"))));
}
