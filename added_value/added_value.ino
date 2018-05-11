#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_GFX.h>
//clock
unsigned long curTime = 0;
unsigned long prevTime = 0;
int hours = 12;
int minutes = 35;
int seconds = 0;

//Pulse initializations
int PulseSensorPin = 0; //A0
int pSignal;
int threshold = 560;
unsigned long prevMil = 0;
unsigned long curMil = 0;
unsigned long timeDif = 0;
double BPM = 0.0;
bool firstBeat = false;
bool edge = false;
int x = 0;
int lastx = 0;
int lasty = 0;
int divVal = 22;
const int led = 13;
int counter = 0;
bool button = false;
int statCount = 0;

//Button
const int buttonPin = 4;
int buttonState = 0;

// display display TWI address
#define display_ADDR   0x3C

Adafruit_SSD1306 display(-1);
void setup() {
  // initialize and clear display
  display.begin(SSD1306_SWITCHCAPVCC, display_ADDR);
  display.clearDisplay();
  display.display();
  Serial.begin(9600);
  // display a line of text
  display.setTextSize(3);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Hello, world!");

  // update display with all of the above graphics
  display.display();
  display.clearDisplay();

}

void loop() {
    buttonState = digitalRead(buttonPin);
  if(buttonState == HIGH){
    if(button == true){
      button = false;
      display.clearDisplay();
      delay(200);
    }
    else{
      button = true;
      display.clearDisplay();
      delay(200);
    }
  }
  //clock
curTime = millis()/1000;
  seconds = curTime - prevTime;
  if(seconds == 60){
    prevTime = curTime;
    minutes++;
  }
  if(minutes == 60){
    minutes = 0;
    hours++;
  }
  if(button == true){
    display.clearDisplay();
    display.setCursor(0,0);
    if(hours < 10){
      display.setCursor(0,10);
      display.print("0");
      display.setCursor(18,10);
      display.print(hours);
    }
    else{
      display.setCursor(0,10);
      display.print(hours);
    }
    if(minutes < 10){
      display.setCursor(45,10);
      display.print("0");
      display.setCursor(63,10);
      display.print(minutes);
    }
    else{
      display.setCursor(45,10);
      display.print(minutes);
    }
    if(seconds < 10){
      display.setCursor(90, 10);
      display.print("0");
      display.setCursor(108,10);
      display.print(seconds);
    }
    else{
      display.setCursor(90,10);
      display.print(seconds);
    }
    display.display();
  }

    //Pulse
    if(button == false){
   if(x>127){
    display.clearDisplay();
    x=0;
    lastx=0;
  }
    }
   pSignal = analogRead(PulseSensorPin);
   //Serial.println(pSignal);
   display.setTextColor(WHITE);
  int y=50-(pSignal/divVal);
  if(button == false){
    display.writeLine(lastx,lasty,x,y,WHITE);
  }
  lasty=y;
  lastx=x;
   //Serial.println(pSignal);
   if(pSignal > threshold && !edge){
    if(!firstBeat){ //detect first heartbeat
      curMil = millis();
      firstBeat = true;
    }
    else{
      prevMil = millis();
      timeDif = prevMil - curMil;
      curMil = prevMil;
      digitalWrite(led,HIGH);
    }
    edge = true;
   }
   if(pSignal < threshold && edge){
    edge = false;
    digitalWrite(led,LOW); 
   }
   if(pSignal < threshold){
    statCount++;
    Serial.println(statCount);
   }
   if(pSignal > threshold){
    statCount = 0;
   }
   Serial.println(timeDif); //just to check why the BPM is a weird value
   BPM = (1.0/timeDif) * 1000.0 * 60.0;
   int number = (int) BPM;   
   Serial.print("BMP: ");
   Serial.println(BPM);
   display.display();
   if(button == false){
     if(statCount > 50){
      display.setTextSize(2);
        display.writeFillRect(0,0,128,15,BLACK);
        display.setCursor(0,0);
        display.print("Call 911");
        display.display();
     }
     else if(number < 150){
        display.setTextSize(2);
        display.writeFillRect(0,0,128,15,BLACK);
        display.setCursor(0,0);
        display.print(number);
        display.setCursor(54, 0);
        display.print("BPM");
        display.display();
     }
     else{
        display.setTextSize(2);
        display.writeFillRect(0,0,128,15,BLACK);
        display.setCursor(0,0);
        display.print("Slow Down");
        display.display();
     }
     x++;
     delay(50);
   }
}
