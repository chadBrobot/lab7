#include <CheapStepper.h>
#include <SPI.h>
#include <SD.h>
File myFile;
#include <Wire.h>
#include <VL53L1X.h>

VL53L1X sensor;

#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,20,4);

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
int loc1 [210] = {}; 
int loc2 [210] = {};
int loc3 [210] = {};
int newloc [210] = {};
int countLoc = 0;
int sum1;
int sum2;
int sum3;
void setup() {
  lcd.init();                      // initialize the lcd 
  lcd.backlight();
  
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
  myFile = SD.open("LOCA.txt");
  sensor.setDistanceMode(VL53L1X::Long);
  sensor.setMeasurementTimingBudget(50000);
  sensor.startContinuous(50);
}

void loop() {
  int sen;
  int but = 1;
  but = digitalRead(button);
  delay(50);

 // use myFile.read() to read from file
 if(count == 1){ //read in file to arrays
  myFile.read();
  myFile.parseInt();
  for(int x=0; x<210; x++){
    loc1[x] = myFile.parseInt();
  }
    
  myFile.read();
  myFile.parseInt();
  for(int x=0; x<210; x++){
    loc2[x] = myFile.parseInt();
  }
  
  myFile.read();
  myFile.parseInt();
  for(int x=0; x<210; x++){
    loc3[x] = myFile.parseInt();
  }
  count=2;
  }
  
  if(but == 0){
    digitalWrite(led, HIGH);
  for (int s=0; s<rot; s++){
    stepper.step(moveClockwise);
    int nStep = stepper.getStep();
    if(s%5 == 0){
    sen = sensor.read()/10;
    Serial.println(sen);
    newloc[countLoc] = sen;
    countLoc++;
    }
  }
  count=3;
  }

  if (count == 3) {
    myFile.close();
    for(int x=0; x<210; x++){
     sum1 += abs((loc1[x]) - (newloc[x]));
    }
    Serial.println(sum1);
    
    for(int x=0; x<210; x++){
     sum2 += abs((loc2[x]) - (newloc[x]));
    }
     Serial.println(sum2);
     
    for(int x=0; x<210; x++){
     sum3 += abs((loc3[x]) - (newloc[x]));
    }
     Serial.println(sum3);
    
   lcd.setCursor(0,0);
   lcd.print("Location: ");
   lcd.setCursor(0,1);
   lcd.print("       ");
   lcd.setCursor(0,1);

   if(sum1<sum2 && sum1<sum3) lcd.print("1");
   else if(sum2<sum1 && sum2<sum3) lcd.print("2");
   else if(sum3<sum2 && sum3<sum1) lcd.print("3");
   else lcd.print("broken");
   while(1){}
  }

 
}
