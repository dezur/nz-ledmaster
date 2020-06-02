#include "ledmaster.h"

extern WiFiUDP UDP;

Ledmaster::Ledmaster(){
    for(int i = 0; i < NUMBER_OF_LEDS * 3; i++){
        leds[i] = 0;
    }
}

void Ledmaster::setLedColor(uint16_t id, uint8_t red, uint8_t green, uint8_t blue){
    leds[id * 3] = red;
    leds[(id * 3) + 1] = green;
    leds[(id * 3) + 2] = blue;
}

void Ledmaster::setLedColor(uint16_t id, uint32_t colorcode){
    uint8_t red, green, blue;
    red = (colorcode >> 16) & 0xFF;
    green = (colorcode >> 8) & 0xFF; 
    blue = (colorcode >> 0) & 0xFF;

    leds[id * 3] = red;
    leds[(id * 3) + 1] = green;
    leds[(id * 3) + 2] = blue;
}

void Ledmaster::ledFadeOut(uint8_t n){
  for(int j = 0; j < n; j++){
    for(int i = 0; i < 180; i++){
      if(leds[i] > 0)
        leds[i]--;
    }
  }
}

uint32_t Ledmaster::Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return ((uint32_t)WheelPos * 3 << 16) | ((uint32_t)255 - WheelPos * 3 <<  8) | 0;
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return ((uint32_t)255 - WheelPos * 3 << 16) | ((uint32_t)0 <<  8) | WheelPos * 3;
  } else {
    WheelPos -= 170;
    return ((uint32_t)0 << 16) | ((uint32_t)WheelPos * 3 <<  8) | 255 - WheelPos * 3;
  }
}

void Ledmaster::sendData(){
    for (int i = 0; i < NUMBER_OF_CLIENTS; i++){
        IPAddress ip(192,168,4,2 + i);
        UDP.beginPacket(ip, 4210); 
        UDP.write(leds, 180);
        UDP.endPacket();
        UDP.stop();
    }
}