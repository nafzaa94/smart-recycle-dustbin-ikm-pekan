// hx711 library
#include "HX711.h"

// lcd i2c
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// servo library
#include <Servo.h>

// ir sensor 
int irsensor = 4;
int valueirsensor = 0;

// ultrasonic sensor
const int trigPin = 5;
const int echoPin = 6;
long duration;
int distance;
int tinggitongsampah = 30;
int valuesampah = 0;

// sensor weight
const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;

HX711 scale;
long reading = 0;

//buzz 
int buzz = 7;

// voltage sensor
int voltageSensor = A0;

float vOUT = 0.0;
double vIN = 0.00;
float R1 = 30000.0;
float R2 = 7500.0;
int ValuevoltageSensor = 0;
int displayvoltage = 0;

// lcd init
LiquidCrystal_I2C lcd(0x27, 16, 2);

// servo init
Servo myservo;

void setup() {
  Serial.begin(9600);
  lcd.begin(); // kalu dy error tukar begin id init()
  lcd.backlight();
  myservo.attach(9);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(irsensor, INPUT);
  pinMode(buzz, OUTPUT);
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);

  myservo.write(0);

  lcd.setCursor(0, 0);
  lcd.print("volt = ");
  lcd.setCursor(0, 1);
  lcd.print("w = ");
  lcd.setCursor(10, 1);
  lcd.print("CLOSE");

}

void loop() {
  voltagesensor();
  hx711();
  ultrasonic();
  valueirsensor = digitalRead(irsensor); // 1,0 true, false HIGH LOW

  if (valueirsensor == LOW){
    myservo.write(90);
    lcd.setCursor(10, 1);
    lcd.print("     ");
    lcd.setCursor(10, 1);
    lcd.print("OPEN");
    delay(5000);
    myservo.write(0);
    lcd.setCursor(10, 1);
    lcd.print("     ");
    lcd.setCursor(10, 1);
    lcd.print("CLOSE");
    }

  //fucntion lcd
  lcd.setCursor(7, 0);
  lcd.print(displayvoltage);
  lcd.setCursor(7, 0);
  lcd.print("     ");
  lcd.setCursor(4, 1);
  lcd.print(reading);
  lcd.setCursor(4, 1);
  lcd.print("      ");
  delay(200);
  

  if (displayvoltage == 100){
    lcd.setCursor(10, 0);
    lcd.print("%");
    }
  else {
    lcd.setCursor(9, 0);
    lcd.print("%");
    }
 
}

void voltagesensor(){
  ValuevoltageSensor = analogRead(voltageSensor); //0-1024 10bit
  vOUT = (ValuevoltageSensor * 5.0) / 1024.0;
  vIN = vOUT / (R2/(R1+R2));

  if (vIN <= 12.60 || vIN >= 12.43){
    displayvoltage = 100;
    }
  else if (vIN <= 12.42 || vIN >= 12.33){
    displayvoltage = 80;
    }
  else if (vIN <= 12.32 || vIN >= 12.21){
    displayvoltage = 60;
    }
  else if (vIN <= 12.20 || vIN >= 11.89){
    displayvoltage = 40;
    }
  else if (vIN <= 11.88 || vIN >= 11.58){
    displayvoltage = 20;
    }
  else if (vIN <= 11.57 || vIN >= 10.5){
    displayvoltage = 0;
    }
  }

void hx711(){
  if (scale.is_ready()) {
    reading = scale.read();
    Serial.print("HX711 reading: ");
    Serial.println(reading);
  } else {
    Serial.println("HX711 not found.");
  }
  }

void ultrasonic(){
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;
  Serial.print("Distance: ");
  Serial.println(distance);

  valuesampah = tinggitongsampah - distance;

  if (valuesampah >= 25){
    digitalWrite(buzz, HIGH);
    }
  else {
    digitalWrite(buzz, LOW);
    }
  }
