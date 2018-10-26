#include <SPI.h>
#include <ServoCds55.h>
#include <pins_arduino.h>
#include <HMC5883L_Simple.h>
#include <Wire.h>       
               
#include <LiquidCrystal_I2C.h>    
LiquidCrystal_I2C lcd(0x27,20,4);  

#include <Servo.h>
#include <SoftwareSerial.h>

Servo esc_left;
Servo esc_right;
Servo esc_back;


ServoCds55 myservo;

int button1 = 12;
int button2 = 13;
int buttonState1 = 0;
int buttonState2 = 0;      
int i;

int buffer;
int bufferPosition;
int black_left = 0;
int black_right = 0;
int black_back = 0;

void setup (void)
{
    Serial.begin (115200);
  digitalWrite(SS, HIGH);  
  SPI.begin ();
  SPI.setClockDivider(SPI_CLOCK_DIV8); 
  pinMode(12,INPUT); 
  pinMode(13,INPUT); 

  esc_left.attach(3,1000, 2000);
  esc_right.attach(4,1000, 2000);
  esc_back.attach(5,1000, 2000);

    esc_left.write(0);
  esc_right.write(0);  
  esc_back.write(0);
  lcd.init(); 
  lcd.backlight();           
  lcd.clear();
  lcd.setCursor(0,0);
  
  lcd.print("-------Weaver-------");


delay(1500);
}

void loop() 
{ 
  HMC5883L_Simple Compass;
  Compass.SetDeclination(-8, 25, 'W');  //23 25 E
  
  buttonState1 = digitalRead(button1);
  buttonState2 = digitalRead(button2);
  delay(3000);
  
  myservo.setVelocity(90); 
  myservo.write(1,155); myservo.write(2,145);myservo.write(3,155); myservo.write(4,145);myservo.write(5,155);myservo.write(6,145);delay(1000);
  delay(3000);
  myservo.write(1,0);myservo.write(3,0);myservo.write(5,0);
  delay(1000);
  myservo.write(2,300);myservo.write(4,300);myservo.write(6,300);
  delay(2000);
  buttonState1 = 0;
  while(1)
  { 
     buttonState1 = digitalRead(button1);
     buttonState2 = digitalRead(button2);
    
    if(buttonState1 == HIGH)
    { 
     
      while(1)
      { 
        buttonState1 = digitalRead(button1);
        buttonState2 = digitalRead(button2);
        float heading = Compass.GetHeadingDegrees();
        
        lcd.setCursor(0,1);
        lcd.print("Heading : ");
        lcd.print(heading);
        
        if(heading >= 260 && heading <= 360)
        { get_action_center();
          esc_back.write(20);esc_left.write(00);esc_right.write(00);  }
        
        if(heading >= 80 && heading <= 220)
        {  get_action_right(); 
           esc_back.write(00); esc_left.write(20); esc_right.write(00);  }
             
        if(heading <= 80 && heading >= 0)
        {  get_action_left(); 
           esc_back.write(00); esc_left.write(00); esc_right.write(20);  }  
                       
        if(buttonState2 == HIGH && buttonState1 == HIGH )
        {
    
           esc_back.write(00);esc_left.write(00);esc_right.write(00);    
           delay(1000);
           myservo.setVelocity(90); 
           myservo.write(1,155); myservo.write(2,145);myservo.write(3,155); myservo.write(4,145);myservo.write(5,155);myservo.write(6,145);delay(1000);
           delay(1000);
           myservo.write(1,0);myservo.write(3,0);myservo.write(5,0);
           delay(1000);
           myservo.write(2,300);myservo.write(4,300);myservo.write(6,300);
           delay(1000);
           buttonState1 = 0;
           buttonState2 = 0;
           break;
          
        }


        
      }
      
      
    }
    else;
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  if(buttonState2 == HIGH)
  { 

    while(1)
    {
    
  
      buttonState1 = digitalRead(button1);
      buttonState2 = digitalRead(button2);
      float heading = Compass.GetHeadingDegrees();    
      lcd.setCursor(0,1);
      lcd.print("Heading : ");
      lcd.print(heading);
      if(buttonState1 == LOW && buttonState2 ==LOW)
      {
       get_human_rescue_form();       
      }

      if(buttonState1 == HIGH && buttonState2 == HIGH)
      {
         esc_back.write(00);esc_left.write(00);esc_right.write(00);    
         delay(1000);
         myservo.setVelocity(90); 
         myservo.write(1,155); myservo.write(2,145);myservo.write(3,155); myservo.write(4,145);myservo.write(5,155);myservo.write(6,145);delay(1000);
         delay(1000);
         myservo.write(1,0);myservo.write(3,0);myservo.write(5,0);
         delay(1000);
         myservo.write(2,300);myservo.write(4,300);myservo.write(6,300);
         delay(1000);
         buttonState1 = 0;
         buttonState2 = 0;
         break;
      }
  }
    
  }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------
  }
}

 void get_action_center()
 {
  myservo.setVelocity(80); 
           myservo.write(4,285);//cl
           myservo.write(5,15);//cl
           myservo.write(6,285);//cl
           myservo.write(1,15);//cl 
           delay(500);
           myservo.write(2,155);//op
           myservo.write(3,145);//op   
 }

void get_action_right()
{
  myservo.setVelocity(80); 
           myservo.write(6,285);//cl
           myservo.write(3,15);//cl
           myservo.write(2,285);//cl
           myservo.write(1,15);//cl    
           delay(500);
           myservo.write(4,155);//op
           myservo.write(5,145);//op        
}
void get_action_left()
{
myservo.setVelocity(80); 
          myservo.write(4,285);//cl
           myservo.write(5,15);//cl
           myservo.write(2,285);//cl
           myservo.write(3,15);//cl 
           delay(500);  
           myservo.write(6,155);//op
           myservo.write(1,145);//op       
}

void get_human_rescue_form()
{
  myservo.setVelocity(80); 
  myservo.write(2,234);myservo.write(4,234);myservo.write(6,234);
  delay(1000);
  myservo.write(1,58);myservo.write(3,58);myservo.write(5,58);  
  delay(2000); 
  
}


