#include <unity.h>
#include <Arduino.h>

// Declare the tasks
extern void FollowLine(void *);
extern void CarBrake(void *);
extern void CarEnergy(void *);
extern void AutoLight(void *);
extern void BeltAndDoor(void *);

void setUp()
{
    // Set up 
}

void testFollowLine()
{
    digitalWrite(L, HIGH);
    digitalWrite(M, LOW);
    digitalWrite(Ri, HIGH);

    xSemaphoreTake(xSemaphore, portMAX_DELAY);

    xSemaphoreGive(xSemaphore);

    TEST_ASSERT_EQUAL(digitalRead(LeftT1), HIGH);
    TEST_ASSERT_EQUAL(digitalRead(LeftT2), LOW);
    TEST_ASSERT_EQUAL(digitalRead(RightT1), LOW);
    TEST_ASSERT_EQUAL(digitalRead(RightT2), HIGH);
}

void testCarBrake()
{
    cm = 300; 
    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    xSemaphoreGive(xSemaphore);

    if (cm < 336 && cm > 250)
    {
        TEST_ASSERT_EQUAL(digitalRead(greenLight), HIGH);
    }
    else
    {
        TEST_ASSERT_EQUAL(digitalRead(greenLight), LOW);
    }
}

void testCarEnergy()
{
    energy = 2.5;

    xSemaphoreTake(xSemaphore, portMAX_DELAY);
    CarEnergy(NULL); 
    xSemaphoreGive(xSemaphore);


    TEST_ASSERT_EQUAL(energy, 2.5); 
    TEST_ASSERT_EQUAL(digitalRead(R), 255);
    TEST_ASSERT_EQUAL(digitalRead(G), 0);
    TEST_ASSERT_EQUAL(digitalRead(B), 0);
}

void testAutoLight()
{
    val = 80; 

    AutoLight(NULL);

    TEST_ASSERT_LESS_THAN(val, threshold);                
    TEST_ASSERT_EQUAL(analogRead(photoresistorPin), val); 
    TEST_ASSERT_EQUAL(analogRead(ledPin), 1023);                
}


void setup()
{
    UNITY_BEGIN(); 
    RUN_TEST(testFollowLine);
    RUN_TEST(testCarBrake);
    RUN_TEST(testCarEnergy);
    RUN_TEST(testAutoLight);
    UNITY_END(); 
}

void loop()
{
    // Nothing here 
}
