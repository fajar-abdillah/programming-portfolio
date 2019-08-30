/*
Author: Abdillah Faisal Nur Fajar
Microcontroller: Arduino Nano
IDE: ArduinoIDE
*/

double sensorValue, lux;
int analogValue;

void setup() 
{
  pinMode(A7, INPUT);
  Serial.begin(9600);
}

void loop() 
{
  analogValue = analogRead(A7);
  sensorValue = (analogRead(A7) * (5.0 / 1023.0));
  lux = sensorValue * 1333;

  Serial.print("Digital Photodiode = ");
  Serial.print(analogValue);
  Serial.print("  |  Voltage = ");
  Serial.print(sensorValue);
  Serial.println(" mV");

  Serial.print("Lux = ");
  Serial.println(lux); 
  delay(500);        
}
