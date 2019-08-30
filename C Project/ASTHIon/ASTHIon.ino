/*
Author: Abdillah Faisal Nur Fajar
Microcontroller: ATTiny-85
IDE: ArduinoIDE
*/

#include <SoftwareSerial.h>
#define TxD 0
#define RxD 5

SoftwareSerial BTSerial (RxD, TxD);

int Anode = 1;
int ADCarus = 2;
int ADCSuhu = 3;
int LED = 4;

long t;
long periode = 3 * 60 * 60 * 1000 * 2;
int adcA, adcS;
double arus, suhu;
int pwm = 127;


void setup() {  
  pinMode(RxD, INPUT);
  pinMode(TxD, OUTPUT);
  setupBlueToothConnection();

  pinMode(ADCarus, INPUT);
  pinMode(ADCSuhu, INPUT);
  pinMode(Anode, OUTPUT);
  pinMode(LED, OUTPUT);
  analogWrite(Anode, pwm);
  digitalWrite(LED, HIGH);
}

void loop() {
  char dataRcv;
  while(1){
    if(BTSerial.available()){
      dataRcv = BTSerial.read();
        if(dataRcv == '1') {
            for (t = 0; t < periode; t++) {
              digitalWrite(LED, LOW);
              
              adcA = analogRead(ADCarus);
              arus = (((adcA / 1024) * 5000) / 185);

              adcS = analogRead(ADCSuhu);
              
              if (arus > 0.3 || suhu > 36) {
                pwm -= 5;
              }
  
              analogWrite (Anode, pwm);

              BTSerial.print(arus);
              BTSerial.print("|");
              BTSerial.print(suhu);  

              digitalWrite(LED, HIGH);
              delay(500);
          }
        }
    }
  }
}

void setupBlueToothConnection()
{  
  BTSerial.begin(9600); 
  BTSerial.print("\r\nAT\r\n");
  
  delay(2000); 
  BTSerial.flush();
}
