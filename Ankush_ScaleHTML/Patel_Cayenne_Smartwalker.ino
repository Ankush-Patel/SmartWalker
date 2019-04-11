// This example shows how to connect to Cayenne using an ESP8266 and send/receive sample data.
// Make sure you install the ESP8266 Board Package via the Arduino IDE Board Manager and select the correct ESP8266 board before compiling. 

//#define CAYENNE_DEBUG
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#include <HX711.h>
#define Buzzerout D2 //D2
#define DOUT D4 //D4
#define CLK D5 //D5
#define LEDout D6 //D6

// WiFi network info.
char ssid[] = "ssid";
char wifiPassword[] = "wifiPassword";

// Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT_USERNAME";
char password[] = "MQTT_PASSWORD";
char clientID[] = "CLIENT_ID";

HX711 scale;
float calibration_factor=5220;
float units;
float ounces;
float percent_load = .10;
float person_weight = 170;
float weightmath;
float notifmethod=1;

unsigned long lastMillis = 0;
//================================================================//
//                            SETUP                               //
//================================================================//
void setup() {
	Serial.begin(9600);
	Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  scale.begin(DOUT, CLK);
  pinMode(Buzzerout, OUTPUT);
  digitalWrite(Buzzerout, HIGH);
  scale.set_scale(5220.f)
  scale.tare();
}


//================================================================//
//                            LOOP                                //
//================================================================//
void loop() {
	Cayenne.loop();
  weightmath=(percent_load * person_weight);
  units=(scale.get_units(10), 1)
  if (units < 0){
    units= 0.00
  }
  // Insert an if statement here which select this notification method
  if (units > weightmath){
    digitalWrite(Buzzerout, LOW);
    delay(500);
    digitalWrite(Buzzerout, HIGH);
  }
}

// Default function for sending sensor data at intervals to Cayenne.
// You can also use functions for specific channels, e.g CAYENNE_OUT(1) for sending channel 1 data.
CAYENNE_OUT_DEFAULT()
{
	// Write data to Cayenne here. This example just sends the current uptime in milliseconds on virtual channel 0.
	Cayenne.virtualWrite(0, millis());
	// Some examples of other functions you can use to send data.
	//Cayenne.celsiusWrite(1, 22.0);
	//Cayenne.luxWrite(2, 700);
	//Cayenne.virtualWrite(3, 50, TYPE_PROXIMITY, UNIT_CENTIMETER);
}

// Default function for processing actuator commands from the Cayenne Dashboard.
// You can also use functions for specific channels, e.g CAYENNE_IN(1) for channel 1 commands.
CAYENNE_IN_DEFAULT()
{
	CAYENNE_LOG("Channel %u, value %s", request.channel, getValue.asString());
	//Process message here. If there is an error set an error message using getValue.setError(), e.g getValue.setError("Error message");
}
