#ifndef STREAM1_H
#define STREAM1_H
#include <Arduino.h>
#include "ledmaster.h"

extern Ledmaster ledmaster;

class Stream1{
    public:
        void doVisual(uint16_t n);
    private:
        uint8_t streamData[NUMBER_OF_LEDS];
        uint8_t temp;
        uint32_t timer = 0;;
};

#endif