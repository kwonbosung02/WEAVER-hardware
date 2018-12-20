
#include <pins_arduino.h>

#include <FirebaseArduino.h>

//
// Copyright 2018 KWON BO SUNG.
//
// Licensed under the MIT LISENSE
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.  
//
//  https://github.com/kwonbosung02 
//

#include <ESP8266WiFi.h>

#include <Wire.h>

#include <Servo.h>
#include <SoftwareSerial.h>

#include <string.h>

#include <Arduino.h>
#include <AX12A.h>
#include <pins_arduino.h>

#define WIFI_SSID "---WIFI ID HERE---"
#define WIFI_PASSWORD "---PASSWD HERE---"

#define FIREBASE_HOST "FBHOST HERE"
#define FIREBASE_AUTH "FBHOST AUTH"

//compass sensor
#define Declination       -8.39 //S.Korea. Seoul
#define hmc5883l_address  0x1E

//dynamixel motor control
#define DirectionPin 10
#define BaudRate 1000000

#define ID1 6
#define ID2 14
#define Bat A0

//ADC MODE
ADC_MODE(ADC_VCC);

Servo esc_left;
Servo esc_right;

Servo joint;

int buffer;
int bufferPosition;
int black =0;
	
char array[100] = { 0, };

const int ON_ = 0;
const int OFF_ = 1;

int UP = 512;
int SIDE = 512;

int prev_millis = 0;
/*
  float volt;
  int raw;
  float q;
*/
//13
Servo servo;

void setup()
{
 
 esc_clear();
  Wire.begin(4,5);
  hmc5883l_init();
  
  pinMode(LED_BUILTIN, OUTPUT);
	pinMode(A0, INPUT);

  //AX-12A connection check
  mcuCheck(2);
  ax12a.begin(BaudRate, DirectionPin, &Serial);
  
  netWORK();
  mcuCheck(5);


  //servo move check
  
  servo.attach(13);
  servo.write(50);
  delay(900);
  
  servo.write(110);
  delay(900);
  servo.write(40);
  
  //Serial.begin(9600);

}

int lastsmStatus = 0;

void loop()
{
	int tmp = 0;
	
	//bool isStatus = Firebase.getBool("weavers/-LQ41ltYoZ9fR_01_2e5/isStatus");
	
	int millis_ = 0;
	//bool CapsuleCycle = Firebase.getBool("weavers/-LQ41ltYoZ9fR_01_2e5/isCapsuleCycle");


  ax12a.moveSpeed(ID1, UP, 70);
  ax12a.moveSpeed(ID2, SIDE, 70);
  esc_left.write(0);
  esc_right.write(0);
  int millis__;
  while (1)
  {	
	  int smStatus = Firebase.getInt("weavers/-LQ41ltYoZ9fR_01_2e5/isCapsule");
	  float vccVolt = ((float)ESP.getVcc()) / 1024;
		hmc5883l_GetPosition();	
		dynamixel_Get();
		Bldc_controll();
		Firebase.setInt("weavers/-LQ41ltYoZ9fR_01_2e5/energy", (((vccVolt*1.281) * 100) / 4.2));
		
		if (smStatus == 1)
		{
			millis__ = millis();
			servo.write(5);
			while (millis() - millis__ <= 113);
			servo.write(40);
			while (millis() - millis__ <= 350);
		}
		else if(smStatus == 0)
		{
			servo.write(40);
		}

		/*
		if (smStatus != lastsmStatus)
		{
			if (smStatus == 1)
			{
				servo.write(15);
				delay(180); servo.write(95);
				delay(180);
				

			}
			else;
			delay(50);
		}
		lastsmStatus == smStatus;		
		*/



		



	/*
	     ADC_MODE(ADC_VCC);
void setup (void)
{
pinMode(A0,INPUT);
Serial.begin(9600);
}

void loop() 
{ 
 
float vccVolt = ((float)ESP.getVcc())/1024;

Serial.println((((vccVolt*1.281)*100)/4.2));
delay(1000);
  }
//---------------------
	*/
		
  }

	
}

//-------------------------------------------------------------------------------------------------------//
void esc_clear(){
  esc_left.attach(12,1000, 2000);
  esc_right.attach(14, 1000, 2000);

  Serial.setTimeout(50);
  bufferPosition = 0 ;
  esc_left.write(0);
  esc_right.write(0);
}
//-------------------------------------------------------------------------------------------------------//
void netWORK()
{
  delay(200); 

  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print("connecting..");delay(500);
  }
  Serial.println(WiFi.localIP());
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
//-------------------------------------------------------------------------------------------------------//
void mcuCheck(int a)
{
	int i = 0; 
	for (i = 0; i < a; i++)
	{
		digitalWrite(LED_BUILTIN, ON_);
		delay(300);
		digitalWrite(LED_BUILTIN, OFF_);
		delay(300);
	}
}
//-------------------------------------------------------------------------------------------------------//

void hmc5883l_init(){  
  Wire.beginTransmission(hmc5883l_address);
  Wire.write(0x00);
  Wire.write(0x70); 
  Wire.write(0xA0); 
  Wire.write(0x00); 
  Wire.endTransmission();
  delay(500);
}
//-------------------------------------------------------------------------------------------------------//
int hmc5883l_GetHeading(){
  int16_t x, y, z;
  double Heading;
  Wire.beginTransmission(hmc5883l_address);
  Wire.write(0x03);
  Wire.endTransmission();

  Wire.requestFrom(hmc5883l_address, 6);
  x = (((int16_t)Wire.read()<<8) | (int16_t)Wire.read());
  z = (((int16_t)Wire.read()<<8) | (int16_t)Wire.read());
  y = (((int16_t)Wire.read()<<8) | (int16_t)Wire.read());


  Heading = atan2((double)y, (double)x) + Declination;
  if (Heading>2*PI) 
   Heading = Heading - 2*PI;
  if (Heading<0)    
   Heading = Heading + 2*PI;
  return (Heading* 180 / PI);
  //https://cdn-shop.adafruit.com/datasheets/HMC5883L_3-Axis_Digital_Compass_IC.pdf

}
//-------------------------------------------------------------------------------------------------------//
void hmc5883l_GetPosition(){
	char array1[20] = "\0";
	if( -22 <= hmc5883l_GetHeading() && hmc5883l_GetHeading() <23){
		return Firebase.setString("weavers/-LQ41ltYoZ9fR_01_2e5/compass", "North");
    }
	if( -67 <= hmc5883l_GetHeading() && hmc5883l_GetHeading() <-22){
		return Firebase.setString("weavers/-LQ41ltYoZ9fR_01_2e5/compass", "Northwest");
	}
	if( -133 <= hmc5883l_GetHeading() && hmc5883l_GetHeading() <-68){
		return Firebase.setString("weavers/-LQ41ltYoZ9fR_01_2e5/compass", "West");
	}
	if( -179 <= hmc5883l_GetHeading() && hmc5883l_GetHeading() <-133){
		return Firebase.setString("weavers/-LQ41ltYoZ9fR_01_2e5/compass", "SouthWest");
	}
	if( -225 <= hmc5883l_GetHeading() && hmc5883l_GetHeading() <-179){
		return Firebase.setString("weavers/-LQ41ltYoZ9fR_01_2e5/compass", "South");
	}
	if( -271 <= hmc5883l_GetHeading() && hmc5883l_GetHeading() <-226){
		return Firebase.setString("weavers/-LQ41ltYoZ9fR_01_2e5/compass", "Southeast");
	}
	if( -300 <= hmc5883l_GetHeading() && hmc5883l_GetHeading() <-272){
		return Firebase.setString("weavers/-LQ41ltYoZ9fR_01_2e5/compass", "East");
	}
	if( 24 <= hmc5883l_GetHeading() && hmc5883l_GetHeading() <-60){
		return Firebase.setString("weavers/-LQ41ltYoZ9fR_01_2e5/compass", "Northeast");
	}
}
//-------------------------------------------------------------------------------------------------------//
void  dynamixel_Get()
{
	int get_cam = Firebase.getInt("weavers/-LQ41ltYoZ9fR_01_2e5/camPose");
	if (get_cam == 0)
	{
		UP = 512;
		SIDE = 512;
	}
	else if (get_cam == 1)
	{
		UP = 352;
		SIDE = SIDE;
	}
	else if (get_cam == 2)
	{
		UP = UP;
		SIDE = 642;
	}
	else if (get_cam == 3)
	{
		UP = 662;
		SIDE = SIDE;
	}
	else if (get_cam == 4)
	{
		UP = UP;
		SIDE = 382;
	}
	ax12a.moveSpeed(ID2, UP, 90);
	ax12a.moveSpeed(ID1, SIDE, 90);
}

void Bldc_controll(){

	bool left_BLDC = Firebase.getBool("weavers/-LQ41ltYoZ9fR_01_2e5/motor_L");
	bool right_BLDC = Firebase.getBool("weavers/-LQ41ltYoZ9fR_01_2e5/motor_R");


	if (left_BLDC == true)
	{

		esc_left.write(55);

	}
	else if (left_BLDC == false)
	{
		esc_left.write(0);
	}
	if (right_BLDC == true)
	{
		esc_right.write(55);
	}
	else if (right_BLDC == false)
	{
		esc_right.write(0);
	}

}
