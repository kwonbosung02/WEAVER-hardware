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


#include <pins_arduino.h>
#include <FirebaseArduino.h>

#include <Wire.h>
#include <ESP8266WiFi.h>

#define WIFI_SSID "---WIFI ID HERE---"
#define WIFI_PASSWORD "---PASSWD HERE---"

#define FIREBASE_HOST "FBHOST HERE"
#define FIREBASE_AUTH "FBHOST AUTH"

#define Left "/ ROOT / TREE /motor_L"

#define Right "/ ROOT / TREE /motor_R"

//pinmode setting
#define left_key 13
#define right_key 15

//var about cam signal
#define cam_up 5
#define cam_left 4
#define cam_down 12
#define cam_right 14

//nodemcu led control on and off
const int ON = 0;
const int OFF = 1;

//set up
void setup()
{
	Serial.begin(9600);
 
	pinMode(LED_BUILTIN, OUTPUT);
	
	pinMode(left_key, INPUT);
	pinMode(right_key, INPUT);

	pinMode(cam_left, INPUT);
	pinMode(cam_right, INPUT);
	pinMode(cam_up, INPUT);
	pinMode(cam_down, INPUT);

  //wifi connect, database connect check
	mcuCheck(2);
	netWORK();
	mcuCheck(3);

}

int last_seen_L = 0;

void loop()
{
  //bldc signal
	give_Signal_bldc();

 //cam signal
	if (digitalRead(cam_left) == HIGH)
		Firebase.setInt("/ ROOT / TREE /camPose", 2);
   
	if (digitalRead(cam_right) == HIGH)
		Firebase.setInt("/ ROOT / TREE /camPose", 4);
   
	if (digitalRead(cam_up) == HIGH)
		Firebase.setInt("/ ROOT / TREE /camPose", 1);
   
	if (digitalRead(cam_down) == HIGH)
		Firebase.setInt("/ ROOT / TREE /camPose", 3);
   
	if (digitalRead(cam_left) == LOW &&
	      digitalRead(cam_right) == LOW && 
	      digitalRead(cam_up) == LOW && 
	      digitalRead(cam_down) == LOW)
		      Firebase.setInt("/ ROOT / TREE /camPose", 0);
}

//-------------------------------------------------------------------------------------------------------//
void netWORK()
{
	delay(200);

	WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
 
	while (WiFi.status() != WL_CONNECTED)
	{
		Serial.print("connecting.."); 
		
		delay(500);
	}
 
	Serial.println(
	            WiFi.localIP()
	                );
 
	Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

}
//-------------------------------------------------------------------------------------------------------//
void mcuCheck(int a)
{
	int i = 0;
	for (i = 0; i < a; i++)
	{
		digitalWrite(LED_BUILTIN, ON);
		delay(300);
   
		digitalWrite(LED_BUILTIN, OFF);
		delay(300);
	}
}
//-------------------------------------------------------------------------------------------------------//
void give_Signal_bldc()
{
	if (digitalRead(left_key) == HIGH)
	{
		Firebase.setBool(Left,true );
		delay(300);
	}
 
	else
	{
		Firebase.setBool(Left, false);
		delay(300);
	}


	if (digitalRead(right_key) == HIGH)
	{

		Firebase.setBool(Right, true);

		delay(300);
	}
 
	else
	{
		Firebase.setBool(Right, false);
		delay(300);
	}
}
