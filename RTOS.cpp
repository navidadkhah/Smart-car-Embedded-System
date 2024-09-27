#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <Adafruit_LiquidCrystal.h>
#include <semphr.h>

// Declare a semaphore handle
SemaphoreHandle_t xSemaphore;

void FollowLine(void *param);  // Follow Line --> Priority: 4
void CarBrake(void *param);    // Brake Car --> Priority: 4
void CarEnergy(void *param);   // Energy --> Priority: 3
void AutoLight(void *param);   // DayLight --> Priority: 2
void BeltAndDoor(void *param); // Seat Belt and Door --> Priority: 1

// Create Task Handlers for each Task
TaskHandle_t AutoLight_handle;
TaskHandle_t CarBrake_handle;
TaskHandle_t CarEnergy_handle;
TaskHandle_t FollowLine_handle;
TaskHandle_t BeltAndDoor_handle;

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
unsigned int startTime = 1500;
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
  // Initial Serial Rate
  Serial.begin(9600);

  // Create Tasks with their priorities
  xTaskCreate(FollowLine, "FollowLine", 128, NULL, 4, &FollowLine_handle);
  xTaskCreate(CarBrake, "CarBrake", 128, NULL, 4, &CarBrake_handle);
  xTaskCreate(CarEnergy, "CarEnergy", 128, NULL, 3, &CarEnergy_handle);
  xTaskCreate(AutoLight, "AutoLight", 128, NULL, 2, &AutoLight_handle);
  xTaskCreate(BeltAndDoor, "BeltAndDoor", 128, NULL, 1, &BeltAndDoor_handle);

  // Create a Semaphore
  xSemaphore = xSemaphoreCreateMutex();

  // Auto Light sesnsor and light bulb
  pinMode(greenLight, OUTPUT);
  pinMode(ledPin, OUTPUT);

  // LCD initilization
  lcd_4.begin(16, 2);
  lcd_4.print("ENERGY: ");
  lcd_4.setCursor(0, 1);
  lcd_4.print(energy);

  // Set RGB for RGB LED
  pinMode(R, OUTPUT);
  pinMode(G, OUTPUT);
  pinMode(B, OUTPUT);

  // Set the reset buttun
  pinMode(reset, INPUT_PULLUP);

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
  // nothing to do here
}

void FollowLine(void *param)
{
  (void)param;
  while (1)
  {
    xSemaphoreTake(xSemaphore,portMAX_DELAY);
    // forward
    if ((digitalRead(L) == 1) && (digitalRead(M) == 0) && (digitalRead(Ri) == 1));
    digitalWrite(LeftT1, HIGH);
    digitalWrite(LeftT2, LOW);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, HIGH);

    // backwards
    if ((digitalRead(L) == 1) && (digitalRead(M) == 1) && (digitalRead(Ri) == 1));
    digitalWrite(LeftT1, LOW);
    digitalWrite(LeftT2, HIGH);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, HIGH);

    // right
    if ((digitalRead(L) == 0) && (digitalRead(M) == 1) && (digitalRead(Ri) == 1));
    digitalWrite(LeftT1, HIGH);
    digitalWrite(LeftT2, LOW);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, HIGH);

    // left
    if ((digitalRead(L) == 1) && (digitalRead(M) == 1) && (digitalRead(Ri) == 0));
    digitalWrite(LeftT1, HIGH);
    digitalWrite(LeftT2, LOW);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, HIGH);

    // stop
    if ((digitalRead(L) == 0) && (digitalRead(M) == 0) && (digitalRead(Ri) == 0));
    digitalWrite(LeftT1, LOW);
    digitalWrite(LeftT2, LOW);
    digitalWrite(RightT1, LOW);
    digitalWrite(RightT2, LOW);

    xSemaphoreGive(xSemaphore);
  }
}

void CarBrake(void *param)
{
  (void)param;
  while (1)
  {
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
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
      digitalWrite(LeftT1, LOW);
      digitalWrite(LeftT2, LOW);
      digitalWrite(RightT1, LOW);
      digitalWrite(RightT2, LOW);
    }
    xSemaphoreGive(xSemaphore);
  }
}

void CarEnergy(void *param)
{
  (void)param;
  while (1)
  {
    if (energy != 0)
    {
      unsigned long currentMillis = millis();
      if (currentMillis > startTime)
      {
        startTime += startTime;
        energy -= decrease;
        if (energy == 10)
        {
          // setColor(255, 255, 255);
          digitalWrite(R, 255);
          digitalWrite(G, 255);
          digitalWrite(B, 255);
        }
        if (energy == 7.5)
        {
          // setColor(0, 255, 0);
          digitalWrite(R, 0);
          digitalWrite(G, 255);
          digitalWrite(B, 0);
        }
        if (energy == 5)
        {
          // setColor(255, 255, 0);
          digitalWrite(R, 255);
          digitalWrite(G, 255);
          digitalWrite(B, 0);
        }
        if (energy == 2.5)
        {
          // setColor(255, 0, 0);
          digitalWrite(R, 255);
          digitalWrite(G, 0);
          digitalWrite(B, 0);
        }
        lcd_4.setCursor(0, 1);
        lcd_4.print("     ");
        lcd_4.setCursor(0, 1);
        lcd_4.print(energy);
      }
    }
    if (energy == 0)
    {
      vTaskSuspend(AutoLight_handle);
      vTaskSuspend(CarBrake_handle);
      vTaskSuspend(FollowLine_handle);
      vTaskSuspend(BeltAndDoor_handle);
      if (digitalRead(reset) != 0)
      {
        energy = 10;
        startTime = millis() + 1500;
        lcd_4.begin(16, 2);
        lcd_4.print("ENERGY: ");
        lcd_4.setCursor(0, 1);
        lcd_4.print(energy);
        vTaskResume(AutoLight_handle);
        vTaskResume(CarBrake_handle);
        vTaskResume(FollowLine_handle);
        vTaskResume(BeltAndDoor_handle);
      }
    }
  }
}

void AutoLight(void *param)
{
  (void)param;
  while (1)
  {
    val = analogRead(photoresistorPin);
    if (val < threshold)
    {
      analogWrite(ledPin, 1023);
    }
    else
    {
      analogWrite(ledPin, 0);
    }
  }
}

void BeltAndDoor(void *param)
{
  (void)param;
  while (1)
  {
    Serial.print("Task1");
  }
}

// set color on rgb led
void setColor(int redValue, int greenValue, int blueValue)
{
  digitalWrite(R, redValue);
  digitalWrite(G, greenValue);
  digitalWrite(B, blueValue);
}