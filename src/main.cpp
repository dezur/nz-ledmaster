#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include "U8g2lib.h"
#include "ledmaster.h"
#include "audiomaster.h"

// LED strip visual effects
#include "visual\vu1.h"
#include "visual\beatpulse1.h"
#include "visual\stream1.h"

U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
WiFiUDP UDP;
Ledmaster ledmaster;
Audiomaster audiomaster;

// LED strip visual effects
Vu1 vu1;
Beatpulse1 beatpulse1;
Stream1 stream1;

void vTaskAudio( void * pvParameters ){
  for( ;; ){
    audiomaster.loop();
    delay(1);
  }
}

void vTaskVisualSend( void * pvParameters ){
  for( ;; ){
    //beatpulse1.doVisual(audiomaster.filterIn(audiomaster.getAudioLevel()), audiomaster.getBeatLimit());
    stream1.doVisual(audiomaster.getAudioLevel());
    Serial.print(audiomaster.getAudioLevel());
    Serial.print(";");
    Serial.print(vu1.getMaxLevel());
    Serial.print(";");
    Serial.println(audiomaster.filterIn(audiomaster.getAudioLevel()));
    ledmaster.sendData();
    delay(3);
  }
}

void vTaskDisplay(void *pvParameters){
  for(;;){
    u8g2.clearBuffer();
    u8g2.setCursor(0, 18);
    for(int i = 0; i < map(audiomaster.filterIn(audiomaster.getAudioLevel()), 0, 1400, 0, 64); i++){
      u8g2.drawLine(i * 2, 27, i * 2, 31);
    }

    u8g2.sendBuffer();
    delay(3);
  }
}

void setup() {
  u8g2.begin();
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_nokiafc22_tr);
  u8g2.setCursor(0, 7);
  u8g2.print("NZ LEDMASTER");
  u8g2.sendBuffer();
  WiFi.mode(WIFI_AP);
  WiFi.softAP("nz_led", "123456789");

  UDP.begin(4210);
  Serial.begin(115200);
  audiomaster.init();
  audiomaster.setBeatLimit(700);

  xTaskCreatePinnedToCore(vTaskAudio, "AudioTask", 10000, NULL, 5, NULL, 0);
  xTaskCreatePinnedToCore(vTaskVisualSend, "VisualSendTask", 10000, NULL, 5, NULL, 1);
  xTaskCreatePinnedToCore(vTaskDisplay, "DisplayTask", 10000, NULL, 3, NULL, 1);
}

void loop() {
  
}
