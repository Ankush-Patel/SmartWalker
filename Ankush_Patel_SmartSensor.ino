#include <ESP8266WiFi.h> //added when connected to esp8266 and the following. 
#include <HX711.h>
#define LED_Pin 8 //D8
#define DOUT 2 //D2
#define CLK 3 //D3

HX711 scale(DOUT, CLK);
float calibration_factor=5220;
float units;
float ounces;
float percent_load = .10;
float person_weight = 170;
float weightmath;

void setup() {
  Serial.begin(9600);
  pinMode(LED_Pin, OUTPUT);
  digitalWrite(LED_Pin, HIGH);

  scale.set_scale();
  scale.tare();

}

void loop() {
  scale.set_scale(calibration_factor);
  units=scale.get_units(), 10;
  if (units < 0){
  units = 0.00;
  }

  weightmath=(percent_load * person_weight);
  if (units > weightmath){
    digitalWrite(LED_Pin, LOW);
    delay(500);
    digitalWrite(LED_Pin, HIGH);
  }
  Serial.print("Reading: ");
  Serial.print(units);
  Serial.print(" Pounds");
  Serial.println();
  Serial.print("Current Load Threshold: ")
  Serial.print(weightmath);
  Serial.println();
}
