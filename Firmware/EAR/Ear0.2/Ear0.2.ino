const int earID = 1;

void setup(){
  Serial.begin(9600); //открываем порт для связи с ПК
  nrfStart();
}

void loop() {
    Serial.println(getCondition());
}
