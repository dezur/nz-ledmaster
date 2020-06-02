#ifndef LEDMASTER_H
#define LEDMASTER_H
#include <Arduino.h>
#include <WiFiUDP.h>

#define NUMBER_OF_LEDS 60
#define NUMBER_OF_CLIENTS 2

class Ledmaster{

    public:
        Ledmaster();
        void setLedColor(uint16_t id, uint8_t red, uint8_t green, uint8_t blue);
        void setLedColor(uint16_t id, uint32_t colorcode);
        void ledFadeOut(uint8_t n);
        uint32_t Wheel(byte WheelPos); 
        void sendData();

    private:
        uint8_t leds[NUMBER_OF_LEDS * 3];
        
};

#endif