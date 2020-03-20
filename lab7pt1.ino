#include <CheapStepper.h>
#include <SPI.h>
#include <SD.h>
File myFile;
#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

CheapStepper stepper (26,27,28,29); 
//CheapStepper stepper;
// here we declare our stepper using default pins:
// arduino pin <--> pins on ULN2003 board:
// 26 <--> IN1
// 27 <--> IN2
// 28 <--> IN3
// 29 <--> IN4
boolean moveClockwise = true;
int rot = 1046;
#define button 25
#define led 14
int count = 1;
void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // use 400 kHz I2C
  sensor.setTimeout(500);
  if (!sensor.init())
  {
    Serial.println("Failed to detect and initialize sensor!");
    while (1);
  }
   if (!SD.begin(53)) {
    Serial.println("initialization failed!");
    while (1);
  } 
  pinMode(button, INPUT);
  pinMode(led, OUTPUT);
  Serial.println("Ready to start moving!");
  myFile = SD.open("rotate.txt", FILE_WRITE);
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);
  sensor.startContinuous(50);
}

void loop() {
  int sen;
  int but = 1;
  but = digitalRead(button);
  delay(50);
  if(but == 0){
    digitalWrite(led, HIGH);
    myFile.println("Location " + count);
  for (int s=0; s<rot; s++){
    stepper.step(moveClockwise);
    int nStep = stepper.getStep();
    if(s%5 == 0){
    sen = sensor.read()/10;
    Serial.println(sen);
    myFile.println(sen);
    }
  }
  count++;
  }

  if (count == 4) {
    myFile.close();
    while(1){}
  }
  
}
