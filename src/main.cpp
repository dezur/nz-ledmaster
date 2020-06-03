#include <Arduino.h>
#include <WiFi.h>
#include <WiFiUDP.h>
#include "AiEsp32RotaryEncoder.h"
#include "U8g2lib.h"
#include "ledmaster.h"
#include "audiomaster.h"

// LED strip visual effects
#include "visual\vu1.h"
#include "visual\beatpulse1.h"
#include "visual\stream1.h"

AiEsp32RotaryEncoder rotaryEncoder = AiEsp32RotaryEncoder(18, 19, 5, -1);
U8G2_SSD1306_128X32_UNIVISION_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);
WiFiUDP UDP;
Ledmaster ledmaster;
Audiomaster audiomaster;

// LED strip visual effects
Vu1 vu1;
Beatpulse1 beatpulse1;
Stream1 stream1;

void rotary_loop() {
	//first lets handle rotary encoder button click
	if (rotaryEncoder.currentButtonState() == BUT_RELEASED) {
		//we can process it here or call separate function like:
		//rotary_onButtonClick();
	}

	//lets see if anything changed
	int16_t encoderDelta = rotaryEncoder.encoderChanged();
	
	//optionally we can ignore whenever there is no change
	if (encoderDelta == 0) return;
	
	//for some cases we only want to know if value is increased or decreased (typically for menu items)
	//if (encoderDelta>0) Serial.print("+");
	//if (encoderDelta<0) Serial.print("-");

	//for other cases we want to know what is current value. Additionally often we only want if something changed
	//example: when using rotary encoder to set termostat temperature, or sound volume etc
	
	//if value is changed compared to our last read
	if (encoderDelta!=0) {
		//now we need current value
		int16_t encoderValue = rotaryEncoder.readEncoder();
		//process new value. Here is simple output.
		//Serial.print("Value: ");
		//Serial.println(encoderValue);
	} 
	
}

void vTaskAudio( void * pvParameters ){
  for( ;; ){
    audiomaster.loop();
    delay(1);
  }
}

void vTaskVisualSend( void * pvParameters ){
  for( ;; ){
    switch(rotaryEncoder.readEncoder()){
      case 0 : {
        vu1.doVisual(audiomaster.getAudioLevel());
        break;
      }

      case 1 :{
        beatpulse1.doVisual(audiomaster.filterIn(audiomaster.getAudioLevel()), audiomaster.getBeatLimit());
        break;
      }

      case 2 :{
        stream1.doVisual(audiomaster.filterIn(audiomaster.getAudioLevel()));
        break;
      }
    }
    
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
    rotary_loop();
    
    u8g2.clearBuffer();
    u8g2.setCursor(0, 18);
    u8g2.print(rotaryEncoder.readEncoder());
    for(int i = 0; i < map(audiomaster.filterIn(audiomaster.getAudioLevel()), 0, 1400, 0, 64); i++){
      u8g2.drawLine(i * 2, 27, i * 2, 31);
    }

    u8g2.sendBuffer();
    delay(3);
  }
}

void setup() {
  rotaryEncoder.begin();
	rotaryEncoder.setup([]{rotaryEncoder.readEncoder_ISR();});
  rotaryEncoder.setBoundaries(0, 2, true);

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