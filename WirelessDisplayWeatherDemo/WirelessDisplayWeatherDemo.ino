#include <NextionText.h>
#include <NextionPage.h>
#include <Nextion.h>
#include "WiFiManager.h"  
#include <ESP8266WebServer.h>
#include "Adafruit_SHT31.h"
#include <SFE_BMP180.h>
#define nextion Serial
Nextion nex(nextion);
ESP8266WebServer server ( 80 );

Adafruit_SHT31 sht31 = Adafruit_SHT31();
float t;
float h;
char temp[10];
char hum[10];
  
void localSetText(int page, int componentid, char* componentname, char* displayText){
  NextionText text(nex, page, componentid, componentname);
  text.setText(displayText);  
}

void handleRoot() {
  server.send ( 200, "text/plain", "NanoMesher wirelesss display" );
}

void handleTemp() {
  server.send ( 200, "text/plain", temp );
}

void handleHumid() {
  server.send ( 200, "text/plain", hum );
}



void setup() {
  //Serial.begin(9600); 
  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }

  WiFiManager wifiManager;
  //wifiManager.resetSettings();
  
  if(!wifiManager.autoConnect("NM-WirelessDisplay")) {
    //Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  } 

  nextion.begin(9600);
  nex.init();
  server.on ( "/",handleRoot);
  server.on ( "/humidity",handleHumid);
  server.on ( "/temperature",handleTemp);

  server.begin();

}

void loop() {
  
  server.handleClient();

  t = sht31.readTemperature();
  h = sht31.readHumidity();

  dtostrf(t,1,1,temp);
  localSetText(0,2,"tempt",temp);


  dtostrf(h,1,1,hum);
  localSetText(0,3,"humt",hum);

  
}

