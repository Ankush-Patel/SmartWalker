#define BLYNK_PRINT Serial


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <HX711_ADC.h>
#define Buzzerout D2 //D2
#define DOUT D4 //D4
#define CLK D5 //D5
#define LEDout D6 //D6
//HX711 scale;
HX711_ADC LoadCell(DOUT, CLK);
int eepromAdress = 0;
long t;

float calibration_factor=1000;
float percent_load;
float person_weight;
float units;
int ounces;
float weightmath;
int notifmethod=1;
BlynkTimer timer1;
// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "20d408efd832418d92a30afe2eb8759f";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "Zeiny's iPhone";
char pass[] = "formulaone";

BLYNK_WRITE(V1)
{
  percent_load = param.asInt();
  Serial.println(percent_load);
}

BLYNK_WRITE(V2)
{
  person_weight = param.asInt();
  Serial.println(person_weight);
}

void getscaleweight() {
  LoadCell.update();
  units = LoadCell.getData();
  
 // units = scale.read();
  Serial.print("Reading: ");
  Serial.println(units);
}
void buzzercheck(){
  if (units > weightmath){;
  Serial.print("Threshold: ");
  Serial.println(weightmath);
  digitalWrite(Buzzerout, LOW);
  delay(100);
  digitalWrite(Buzzerout, HIGH);
}
}


void sendweight ()
{
 weightmath=((percent_load/100) * person_weight);
 Blynk.virtualWrite(V3, units);
}
void sendthresh() {
  Blynk.virtualWrite(V5, weightmath);
}
void setup()
{
  // Debug console
  float calValue;
  calValue = 696.0;
  Serial.begin(9600);
  Blynk.begin(auth, ssid, pass);
  pinMode(Buzzerout, OUTPUT);
  //scale.begin(DOUT, CLK);
  LoadCell.begin();
  long stabilisingtime = 2000; // tare preciscion can be improved by adding a few seconds of stabilising time
  LoadCell.start(stabilisingtime);
  if (LoadCell.getTareTimeoutFlag()) {
    Serial.println("Tare timeout, check MCU>HX711 wiring and pin designations");
  }
  else {
    LoadCell.setCalFactor(calValue); // set calibration factor (float)
    Serial.println("Startup + tare is complete");
  }
  digitalWrite(Buzzerout, HIGH);
  //scale.tare();
  timer1.setInterval(1000L, getscaleweight);
  timer1.setInterval(500L, buzzercheck);
  timer1.setInterval(1000L, sendweight);
  timer1.setInterval(1000L, sendthresh);
  
  
  

}

void loop()
{
  Blynk.run();
  timer1.run();
}
