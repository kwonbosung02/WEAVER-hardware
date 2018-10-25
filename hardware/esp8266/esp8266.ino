
#include <FirebaseArduino.h>
#include <ESP8266WiFi.h>
#include <Servo.h>
#define WIFI_SSID "[NAME_HERE]"
#define WIFI_PASSWORD "******"

#define FIREBASE_HOST "FIREBASE_HOST_HERE"
#define FIREBASE_AUTH "FIREBASE_AUTH_HERE"
Servo servo;

int ledPower = 16;
int turn_on = 0;
int turn_off = 1;

const int buttonPin1 = 13; 
const int buttonPin2 = 15; 
const int buttonPin3 = 14;

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;

int lastbuttonState1 = 0;
int lastbuttonState2 = 0;
int lastbuttonState3 = 0;
int lastsmStatus = 0;
int R = 0;

void setup() 
{ 
  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
  pinMode(buttonPin3, INPUT);
  
  servo.attach(4);
  pinMode(ledPower,OUTPUT);
  digitalWrite(ledPower, turn_off);

  Serial.begin(9600);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  
  Serial.print("connecting");
  while(WiFi.status() != WL_CONNECTED){Serial.print(".");delay(500);}
  
  Serial.println();
  Serial.print("connected");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  digitalWrite(ledPower,turn_on);
  delay(300);
  digitalWrite(ledPower,turn_off);
  delay(300);
  digitalWrite(ledPower,turn_on);
  delay(300);
  digitalWrite(ledPower,turn_off);
  delay(300);
  digitalWrite(ledPower,turn_on);
  delay(300);
  digitalWrite(ledPower,turn_off);
      servo.write(85) ;

}

void loop() 
{  
  int smStatus = Firebase.getInt("smStatus");
  int capsule = Firebase.getInt("capsule");
  int switch1 = Firebase.getInt("switch"); 
  buttonState1 = digitalRead(buttonPin1);
  buttonState2 = digitalRead(buttonPin2);
  buttonState3 = digitalRead(buttonPin3);
  
  if(capsule == 0 );
  else
  {
    while(capsule != 0)
    { capsule = Firebase.getInt("capsule");
      delay(capsule);
      servo.write(65);
      digitalWrite(ledPower,turn_on); 
      delay(180);
      servo.write(85) ;
      digitalWrite(ledPower,turn_off); 
     if(capsule == 0 )
     {
         servo.write(85) ;break;
     }
    }
  }
  if(smStatus != lastsmStatus)
  {
    if(smStatus == 1)
    {
       servo.write(65);
       delay(180);servo.write(85);
       Serial.println("fdkjdskhgkjdfhskgj");
       
    }
    else;
    delay(50);
  }
  lastsmStatus == smStatus;


 


}
