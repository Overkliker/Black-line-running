#include <Arduino.h>

int PIN_ENA = A1; // Вывод управления скоростью вращения мотора №1
int PIN_ENB = A2; // Вывод управления скоростью вращения мотора №2

int SVET1 = A3; // Вывод управления скоростью вращения мотора №2
int SVET2 = A4; // Вывод управления скоростью вращения мотора №2

#define PIN_IN1 12 // Вывод управления направлением вращения мотора №1
#define PIN_IN2 11 // Вывод управления направлением вращения мотора №1
#define PIN_IN3 10// Вывод управления направлением вращения мотора №2
#define PIN_IN4 9 // Вывод управления направлением вращения мотора №2
uint8_t power = 130; // Значение ШИМ (или скорости вращения)

int povt=130;
int pov=130;
int v=130;

#define LS 5 //Правый сенсор
#define CS 4 //центральный
#define RS 2 //Левый 


#define TRIG 7
#define ECHO 6 


// Определение времени задержки
long getEchoTiming() {
  digitalWrite(TRIG, HIGH); // генерируем 10 мкс импульс запуска
  delayMicroseconds(10);  
  digitalWrite(TRIG, LOW);
  // определение на пине echoPin длительности уровня HIGH, мкс:
  long duration = pulseIn(ECHO, HIGH); 
  return duration;
}

// Определение дистанции до объекта в см
long getDistance() {
  long distacne_cm = getEchoTiming() * 1.7 * 0.01;
  return distacne_cm;
}

void setup() {
  // Установка всех управляющих пинов в режим выхода
  Serial.begin(9600);
  pinMode(PIN_ENA, OUTPUT);
  pinMode(PIN_ENB, OUTPUT);

  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);

  pinMode(LS, INPUT);
  pinMode(RS, INPUT);
  pinMode(CS, INPUT);

  pinMode(TRIG, OUTPUT); // триггер - выходной пин
  pinMode(ECHO, INPUT); // эхо - входной

  digitalWrite(TRIG, LOW);
}

void drive_black_line() { 

  // Serial.println(digitalRead(RCS));
  // Serial.println("Centre");
  // Serial.println(digitalRead(CS));
  // Serial.println("Left");
  // Serial.println(digitalRead(LS));
  // Serial.println("Right");
  // Serial.println(digitalRead(RS));


  long distance = getDistance(); // получаем дистанцию с датчика


  if(distance <= 15) // остановка на контрольной точке
  {

    analogWrite(PIN_ENA, 0);
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);

    analogWrite(PIN_ENB, 0);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);

    delay(3050); //ЗАдержка на 3 секунды у контрольной точки

    analogWrite(PIN_ENA, v);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN1, LOW);

    analogWrite(PIN_ENB, v);
    digitalWrite(PIN_IN4, HIGH);
    digitalWrite(PIN_IN3, LOW);
    delay(2050);
  }

  if((!(digitalRead(LS)) && !(digitalRead(RS)) && digitalRead(CS))) // движение вперед
  {


    analogWrite(PIN_ENA, v);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN1, LOW);

    analogWrite(PIN_ENB, v);
    digitalWrite(PIN_IN4, HIGH);
    digitalWrite(PIN_IN3, LOW);

    Serial.println("run");
  }

  else if((!(digitalRead(LS)) && digitalRead(RS) && digitalRead(CS))
   || (!(digitalRead(LS)) && digitalRead(RS) && !(digitalRead(CS)))) // поворачиваем направо
  {


    analogWrite(PIN_ENA, povt);
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);

    analogWrite(PIN_ENB, pov);
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
    Serial.println("right");
  }

  else if((digitalRead(LS) && !(digitalRead(RS)) && digitalRead(CS))
   || (digitalRead(LS) && !(digitalRead(RS)) && !(digitalRead(CS)))) // поворачиваем налево
  {

    analogWrite(PIN_ENA, pov);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);

    analogWrite(PIN_ENB, povt);
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    Serial.println("left");
  }


  else if(digitalRead(LS) && digitalRead(RS) && digitalRead(CS)){ // остановка когда датчики видят чёрный, для окончания дистанции

    analogWrite(PIN_ENA, 0);
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);

    analogWrite(PIN_ENB, 0);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    delay(3000);

    analogWrite(PIN_ENA, v);
    digitalWrite(PIN_IN2, HIGH);
    digitalWrite(PIN_IN1, LOW);

    analogWrite(PIN_ENB, v);
    digitalWrite(PIN_IN4, HIGH);
    digitalWrite(PIN_IN3, LOW);
    delay(500);
  }
  else{
    
    analogWrite(PIN_ENA, povt);
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);

    analogWrite(PIN_ENB, pov);
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
  }
}


void loop() {
  drive_black_line();
  delay(10);

}