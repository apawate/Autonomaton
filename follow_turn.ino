//     Right motor truth table
//Here are some handy tables to show the various modes of operation.
//  ENB         IN3             IN4         Description  
//  LOW   Not Applicable   Not Applicable   Motor is off
//  HIGH        LOW             LOW         Motor is stopped (brakes)
//  HIGH        LOW             HIGH        Motor is on and turning forwards
//  HIGH        HIGH            LOW         Motor is on and turning backwards
//  HIGH        HIGH            HIGH        Motor is stopped (brakes)   


// **************************************
// 27.3 IPS IS THE TOP SPEEEEEEEEEEEEEEED
// **************************************

#include <math.h>
// define IO pin
#define ENA 5
#define ENB 6
#define IN1 7
#define IN2 8
#define IN3 9
#define IN4 11

//set car speed
boolean left = true;
float speeder = 255;
typedef unsigned char u8;  //Change the name of unsigned char to u8
const int TRIG_PIN = A5;
const int ECHO_PIN = A4;
const int TRIG_PIN_TWO = 12;
const int ECHO_PIN_TWO = 13;
const float i = 2; // Distance between the two sensors
float r = 0; // Angle to turn
float t; // Distance measured by right sensor
float h; // Distance measured by left sensor

void turny(float degree, boolean left) {
  if (left) {
    analogWrite(ENA,speeder);
    analogWrite(ENB,speeder);
    digitalWrite(IN1,LOW);
    digitalWrite(IN2,HIGH);
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,HIGH);
    delay((1160/360.0) * degree); 
  }
  else {
    analogWrite(ENA, speeder);
    analogWrite(ENB, speeder);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay((1160/360.0) * degree);
  }
}

void stoppy()
{
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
}

float Distance_test(int trig, int echo) {
  unsigned long t1;
  unsigned long t2;
  unsigned long pulse_width;
  float cm;
  float inches;

  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  while (digitalRead(echo) == 0);
  t1 = micros();
  while (digitalRead(echo) == 1);
  t2 = micros();
  pulse_width = t2 - t1;

  inches = pulse_width/148.0;

  return (float)inches;
  delay(60);
} 


void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  pinMode(TRIG_PIN_TWO, OUTPUT);
  digitalWrite(TRIG_PIN_TWO, LOW);
  pinMode(IN1,OUTPUT); 
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
  Serial.begin(9600);
  Serial.println("HI");
}

void loop() {
  turny(r, left);
  analogWrite(ENA, speeder);
  analogWrite(ENB, speeder);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
  float a1 = Distance_test(TRIG_PIN, ECHO_PIN);
  delay(100);
  float a2 = Distance_test(TRIG_PIN, ECHO_PIN);
  float a3 = Distance_test(TRIG_PIN_TWO, ECHO_PIN_TWO);
  if (a2 > a3) {
    t = a2;
    h = a3;
    left = true;
  }
  if (a3 > a2) {
    t = a3;
    h = a2;
    left = false;
  }
  if (a3 == a2) {
    t = a2;
    h = a3;
    left = true;
  }
  float traveled = a1 - a2;
  float speedy = traveled * 10;
  speeder = speeder - ((speedy/27.3) * 255);
  r = atan2((t-h), i);
  r = (r/(2 * M_PI )) * 360;
  if (speeder < 0) {
    speeder = 0;
  }
  if (speeder > 255) {
    speeder = 255;
  }
  Serial.print("Turn angle: ");
  Serial.println(r);
  Serial.println(h);
  Serial.println(t);
  if (r > 70) {
    r = 0;
  }
//  Serial.println(a1);
//  Serial.println(a2);
//  Serial.print(speedy);
//  Serial.println(" inches per second");
//
//  if (a2 < 10) {
//    Serial.println("===============BREAK============");
//  }
}
