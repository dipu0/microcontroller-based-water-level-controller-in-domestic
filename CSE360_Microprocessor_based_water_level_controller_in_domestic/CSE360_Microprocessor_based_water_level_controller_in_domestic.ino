#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
const int trigPin = 11;
const int echoPin = 10;

int led1 = 3;
int led2 = 4;
int led3 = 5;
int led4 = A1;
int led5 = A0;

int pbuttonPin = 8;
int pump = 13;


int val = 0; // push value from pin 2
int AutoSenseON = 0;// status
int pushed = 0;//push status

float waterlevel=0;
float maxdist=0;
float currentlevel=0;
long duration;
float distanceCm, distanceInch;
float distance = 0;
void setup() {
  lcd.init();
  lcd.begin(16, 2); // Initializes the interface to the LCD display
  pinMode(pbuttonPin, INPUT_PULLUP);
  lcd.backlight();
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  pinMode(pump, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);

  digitalWrite(pump, LOW);

  Serial.begin(9600);
}
void loop() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm = (duration / 2) / 29.1;
  distanceInch = duration * 0.0133 / 2;

  lcd.setCursor(0,0);
  lcd.print("Water Level: ");
  Serial.print("Water Level: ");
  Serial.print(waterlevel);
  Serial.println(" %");
  if(maxdist==0)
  {
    maxdist=distanceCm;
    }else{
      currentlevel=maxdist-distanceCm;
      }

  waterlevel=(100/maxdist)*currentlevel;

    
  if(waterlevel>=0 && waterlevel<=100)
  {
  lcd.print(waterlevel);
  lcd.print("%");
   
    } 
  val = digitalRead(pbuttonPin);// read the push button value

  if (val == HIGH && AutoSenseON == LOW) {

    pushed = 1 - pushed;
    delay(100);
  }

  AutoSenseON = val;

  if (pushed == HIGH) {
    if (  (waterlevel > 85) && (waterlevel <= 100)   )
    {
      digitalWrite(led1, HIGH);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH);
      digitalWrite(led5, HIGH);
      digitalWrite(pump, HIGH);
      lcd.setCursor(0, 1);
      Serial.println("Pump: OFF");
      lcd.print("Pump: OFF");
    } else if (  (waterlevel > 70) && (waterlevel <= 85)  )
    {

      digitalWrite(led1, LOW);
      digitalWrite(led2, HIGH);
      digitalWrite(led3, HIGH);
      digitalWrite(led4, HIGH);
      digitalWrite(led5, HIGH);

    } else

      if (  (waterlevel > 60) && (waterlevel <= 70)  )
      {

        digitalWrite(led1, LOW);
        digitalWrite(led2, LOW);
        digitalWrite(led3, HIGH);
        digitalWrite(led4, HIGH);
        digitalWrite(led5, HIGH);
      } else

        if (  (waterlevel > 40) && (waterlevel <= 60 )  )
        {

          digitalWrite(led1, LOW);
          digitalWrite(led2, LOW);
          digitalWrite(led3, LOW);
          digitalWrite(led4, HIGH);
          digitalWrite(led5, HIGH);
        } else

          if (  (waterlevel >25) && (waterlevel <= 40)  )
          {

            digitalWrite(led1, LOW);
            digitalWrite(led2, LOW);
            digitalWrite(led3, LOW);
            digitalWrite(led4, LOW);
            digitalWrite(led5, HIGH);
          } else

            if (  waterlevel <25 )// && waterlevel >=0
            {

              digitalWrite(led1, LOW);
              digitalWrite(led2, LOW);
              digitalWrite(led3, LOW);
              digitalWrite(led4, LOW);
              digitalWrite(led5, LOW);

              digitalWrite(pump, LOW);
              Serial.println("Pump: ON");
              lcd.setCursor(0, 1);
              lcd.print("Pump: ON");
            }
  } else {
    Serial.println("Pump Killed");
    lcd.setCursor(0, 1);
    lcd.print("Pump Killed");
    digitalWrite(pump,HIGH);

  }
  delay(1000);
}
