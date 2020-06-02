#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include "ledmaster.h"
#include "audiomaster.h"

#include "visual\vu1.h"
#include "visual\beatpulse1.h"

WiFiUDP UDP;
Ledmaster ledmaster;
Audiomaster audiomaster;

Vu1 vu1;
Beatpulse1 beatpulse1;

void setup() {
  WiFi.mode(WIFI_AP);
  WiFi.softAP("nz_led", "123456789");

  UDP.begin(4210);
  Serial.begin(115200);
  audiomaster.init();
}

void loop() {
  audiomaster.loop();
  //vu1.doVisual(audiomaster.getAudioLevel());
  beatpulse1.doVisual(audiomaster.filterIn(audiomaster.getAudioLevel()));
  Serial.println(audiomaster.filterIn(audiomaster.getAudioLevel()));
  ledmaster.sendData();
  delay(1);
}
