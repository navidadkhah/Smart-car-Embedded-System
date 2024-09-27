#include <Adafruit_LiquidCrystal.h>

// --------- Line Detect Vars ---------
const int LeftT1 = 12;
const int LeftT2 = 11;
const int RightT1 = 9;
const int RightT2 = 10;
// sensors
const int L = 1;
const int M = 5;
const int Ri = 8;
// ------------------------------

// --------- Car Energy Vars ---------
Adafruit_LiquidCrystal lcd_4(0);
// RGB LED
const int R = 2;
const int G = 4;
const int B = 3;
float energy = 10;
float decrease = 2.5;
int startTime = 1500;
const int reset = 13;
// ------------------------------

// --------- Car Brake Vars ---------
int cm = 0;
int greenLight = 6;
// ------------------------------

// --------- Car Daylights Vars ---------
int val = 0;
const int ledPin = A2;
const int photoresistorPin = A3;
int threshold = 100;
// ------------------------------

// --------- Find Distance Function ---------
long readUltrasonicDistance(int triggerPin, int echoPin)
{
  pinMode(triggerPin, OUTPUT); // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return pulseIn(echoPin, HIGH);
}

void setup()
{
  pinMode(greenLight, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // LCD initilization
  lcd_4.begin(16, 2);
  lcd_4.print("ENERGY: ");
  lcd_4.setCursor(0, 1);
  lcd_4.print(energy);
  setColor(255, 255, 255);
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  pinMode(reset, INPUT_PULLUP);
  Serial.begin(9600);

  // Line detect motors
  pinMode(LeftT1, OUTPUT);
  pinMode(LeftT2, OUTPUT);
  pinMode(RightT1, OUTPUT);
  pinMode(RightT2, OUTPUT);

  // IR Sensors
  pinMode(R, INPUT);
  pinMode(M, INPUT);
  pinMode(L, INPUT);
}

void loop()
{
  // digitalWrite(LeftT1, LOW);
  // digitalWrite(LeftT2, LOW);

  if (energy == 0)
  {
    lcd_4.begin(16, 2);
    lcd_4.print("PLEASE CHARGE");
    lcd_4.setCursor(0, 1);
    lcd_4.print(energy);
    analogWrite(ledPin, 0);
    if (digitalRead(reset) != 0)
    {
      energy = 10;
      startTime = millis() + 1500;
      lcd_4.begin(16, 2);
      lcd_4.print("ENERGY: ");
      lcd_4.setCursor(0, 1);
      lcd_4.print(energy);
    }
  }

  if (energy != 0)
  {
    // foward
    if ((digitalRead(L) == 1) && (digitalRead(M) == 0) && (digitalRead(Ri) == 1))
      ;
    digitalWrite(LeftT1, HIGH);
    digitalWrite(LeftT2, LOW);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, HIGH);

    // backwards
    if ((digitalRead(L) == 1) && (digitalRead(M) == 1) && (digitalRead(Ri) == 1))
      ;
    digitalWrite(LeftT1, LOW);
    digitalWrite(LeftT2, HIGH);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, HIGH);

    // right
    if ((digitalRead(L) == 0) && (digitalRead(M) == 1) && (digitalRead(Ri) == 1))
      ;
    digitalWrite(LeftT1, HIGH);
    digitalWrite(LeftT2, LOW);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, HIGH);

    // left
    if ((digitalRead(L) == 1) && (digitalRead(M) == 1) && (digitalRead(Ri) == 0))
      ;
    digitalWrite(LeftT1, HIGH);
    digitalWrite(LeftT2, LOW);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, HIGH);

    // stop
    if ((digitalRead(L) == 0) && (digitalRead(M) == 0) && (digitalRead(Ri) == 0))
      ;
    digitalWrite(LeftT1, LOW);
    digitalWrite(LeftT2, LOW);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, LOW);

    // get the current energy
    unsigned long currentMillis = millis();
    if (currentMillis > startTime)
    {
      startTime += startTime;
      energy -= decrease;
      if (energy == 7.5)
      {
        setColor(0, 255, 0);
      }
      if (energy == 5)
      {
        setColor(255, 255, 0);
      }
      if (energy == 2.5)
      {
        setColor(255, 0, 0);
      }
      lcd_4.setCursor(0, 1);
      lcd_4.print("     ");
      lcd_4.setCursor(0, 1);
      lcd_4.print(energy);
    }
    // -----

    // car brake
    cm = 0.01723 * readUltrasonicDistance(7, 7);
    if (cm < 336 && cm > 250)
    {
      digitalWrite(greenLight, HIGH);
      delay(500);
      digitalWrite(greenLight, LOW);
    }
    else if (cm < 250 && cm > 150)
    {
      digitalWrite(greenLight, HIGH);
      delay(250);
      digitalWrite(greenLight, LOW);
    }
    else if (cm < 150 and cm > 50)
    {
      digitalWrite(greenLight, HIGH);
      delay(100);
      digitalWrite(greenLight, LOW);
    }
    else if (cm < 50)
    {
      digitalWrite(greenLight, HIGH);
    }
    // -----

    // auto light
    val = analogRead(photoresistorPin);
    if (val < threshold)
    {
      int brightness = map(val, 0, threshold, 500, 0);
      analogWrite(ledPin, 1023);
    }
    else
    {
      analogWrite(ledPin, 0);
    }
    // -----

    delay(100);
  }
}

// set color on rgb led
void setColor(int redValue, int greenValue, int blueValue)
{
  digitalWrite(R, redValue);
  digitalWrite(G, greenValue);
  digitalWrite(B, blueValue);
}