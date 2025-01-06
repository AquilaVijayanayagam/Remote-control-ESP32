#include <Arduino.h>

//----- Setup FISA to ESP32
#define CH1 33 // Connect to Channel 1 of FISA 6b – Right Stick, Left/Right
#define CH4 34 // Connect to Channel 4 of FISA 6b – Left Stick, Left/Right (unimplemented)

#define CH2 32 // Connect to Channel 2 of FISA 6b – Right Stick, Up/Down
#define CH3 35 // Connect to Channel 3 of FISA 6b – Left Stick, Up/Down (unimplemented)

#define CH6 36 // Connect to Channel 6 of FISA 6b – Control VRB (unimplemented)
#define CH5 39 // Connect to Channel 5 of FISA 6b – Control VRA (unimplemented)

// Integers to hold the updated values from FISA controllers

int ch1Value;
int ch2Value;
int ch3Value;
int ch4Value;
int ch5Value;

// Boolean to hold the updated values from FISA controllers
// bool ch5Value; maybe bool????(for debugging + -1 headaches)
bool ch6Value;

//----- Setup ESP32 to TB6612FNG

// Motor 1

#define CHM1 21 // Connect to DIR1 of TB6612
#define CHM2 19 // Connect to PWM1 of TB6612

// Motor 2

#define CHM3 18 // Connect to PWM2 of TB6612
#define CHM4 15 // Connect to DIR2 cof TB6612

// Integers to hold the updated values from FISA controllers

int chm1Value;
int chm2Value;
int chm3Value;
int chm4Value;

// Functions for reading the FISA controllers

int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue)
{
  int ch = pulseIn(channelInput, HIGH, 30000);
  if (ch < 100)
    return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
// Read the switch channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue)
{
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}

// Functions for write to Motor controller

void Motor1(int value)
{

  if (value >= 0) //if value is greater than one go forwards
  {
    digitalWrite(CHM1, HIGH);
  }
  if (value < 0) // if value is less than one go forwards
  {
    digitalWrite(CHM1, LOW);
  }

  analogWrite(CHM2, abs(value)); // PWM Speed Control
}


void Motor2(int value)
{

  if (value >= 0)
  {
    digitalWrite(CHM3, HIGH);
  }
  if (value < 0)
  {
    digitalWrite(CHM3, LOW);
  }

  analogWrite(CHM4, abs(value)); // PWM Speed Control
}

// ESP32 setup code

void setup()
{
  // Set up serial monitor to listen for  the FISA controllers

  Serial.begin(115200);

  // Set all inputs

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);

  // Set all Outputs

  pinMode(CHM1, OUTPUT);
  pinMode(CHM2, OUTPUT);
  pinMode(CHM3, OUTPUT);
  pinMode(CHM4, OUTPUT);
}

void loop()
{
  // Get values for each channel

  ch1Value = readChannel(CH1, -100, 100, 0);
  ch2Value = readChannel(CH2, -100, 100, 0);
  ch3Value = readChannel(CH3, -100, 100, -100);
  ch4Value = readChannel(CH4, -100, 100, 0);
  ch5Value = readChannel(CH5, -100, 100, 0);
  ch6Value = readSwitch(CH6, false);

  // Print to Serial Monitor

  Serial.print("Ch1: ");
  Serial.print(ch1Value);
  Serial.print(" | Ch2: ");
  Serial.print(ch2Value);
  Serial.print(" | Ch3: ");
  Serial.print(ch3Value);
  Serial.print(" | Ch4: ");
  Serial.print(ch4Value);
  Serial.print(" | Ch5: ");
  Serial.print(ch5Value);
  Serial.print(" | Ch6: ");
  Serial.println(ch6Value);

  // possible bug solution = flip them hardware or software

  if (ch1Value != 0)
  {
    Motor1(ch1Value);
    Motor2(ch1Value);
  }

  // left or right

  if (ch2Value < 0)
  {
    Motor1(ch1Value);
    Motor2(ch1Value * -1);
  }

  // left or right

  if (ch2Value > 0)
  {
    Motor1(ch1Value * -1);
    Motor2(ch1Value);
  }
}
