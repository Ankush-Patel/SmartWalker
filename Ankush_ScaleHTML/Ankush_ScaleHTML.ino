
#include <ESP8266WiFi.h>
#include <HX711.h>

#define Buzzerout D2 //D2
#define DOUT D4 //D4
#define CLK D5 //D5
#define LEDout D6 //D6

const char WiFiPassword[] = "12345678";
const char AP_NameChar[] = "WIFIWalker";
WiFiServer server(80);

HX711 scale(DOUT, CLK);
float calibration_factor=5220;
float units;
float ounces;
float percent_load = .10;
float person_weight = 170;
float weightmath;
float notifmethod=1;

String header = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n";
String html_1 = "<!DOCTYPE html><html><head><title>LED Control</title></head><body><div id='main'><h2>LED Control</h2>";
String html_2 = "<form id='F1' action='LEDON'><input class='button' type='submit' value='LED ON' ></form><br>";
String html_3 = "<form id='F2' action='LEDOFF'><input class='button' type='submit' value='LED OFF' ></form><br>";
String html_4 = "</div></body></html>";
 
String request = "";
//================================================================
//            SETUP
//================================================================
void setup() {
    boolean conn = WiFi.softAP(AP_NameChar, WiFiPassword);
    server.begin();
 
 // Serial.begin(9600);
  pinMode(Buzzerout, OUTPUT);
  digitalWrite(Buzzerout, HIGH);
  scale.set_scale();
  scale.tare();
  
}
//================================================================
//            LOOP
//================================================================
void loop() {
  // Check if a client has connected
    WiFiClient client = server.available();
    if (!client)  {  return;  }
 
    // Read the first line of the request
    request = client.readStringUntil('\r');
 
   // if       ( request.indexOf("LEDON") > 0 )  { digitalWrite(LED_Pin, HIGH);  }
   // else if  ( request.indexOf("LEDOFF") > 0 ) { digitalWrite(LED_Pin, LOW);   }
 
    client.flush();
 
    client.print( header );
    client.print( html_1 );
    client.print( html_2 );
    client.print( html_3 );
    client.print( html_4);
 
    delay(5);
   scale.set_scale(calibration_factor);
  units=scale.get_units(), 10;
  if (units < 0){
  units = 0.00;
  }

  weightmath=(percent_load * person_weight);
  if (notifmethod = 1) {
  if (units > weightmath){
    digitalWrite(Buzzerout, LOW);
    delay(500);
    digitalWrite(Buzzerout, HIGH);
  }
  }
  if (notifmethod = 2) {
    if (units > weightmath){
      digitalWrite(LEDout, HIGH);
      delay(100);
      digitalWrite(LEDout, LOW);
      delay(100);
      digitalWrite(LEDout, HIGH);
      delay(100);
      digitalWrite(LEDout, LOW);
      delay(100);  
      digitalWrite(LEDout, HIGH);
      delay(100);
      digitalWrite(LEDout, LOW);
      delay(100);
      digitalWrite(LEDout, HIGH);
      delay(100);
      digitalWrite(LEDout, LOW);  
    }
  }
  if (notifmethod = 3){
    if (units > weightmath){
      digitalWrite(Buzzerout, LOW);
      digitalWrite(LEDout, HIGH);
      delay(100);
      digitalWrite(LEDout, LOW);
      delay(100);
      digitalWrite(LEDout, HIGH);
      delay(100);
      digitalWrite(LEDout, LOW);
      delay(100);  
      digitalWrite(LEDout, HIGH);
      delay(100);
      digitalWrite(LEDout, LOW);
      digitalWrite(Buzzerout, HIGH);
    }
  }
  Serial.print("Reading: ");
  Serial.print(units);
  Serial.print(" Pounds");
  Serial.println();
  Serial.print("Current Load Threshold: ");
  Serial.print(weightmath);
  Serial.println();
}
