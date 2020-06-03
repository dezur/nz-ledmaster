#include "stream1.h"

void Stream1::doVisual(uint16_t n){
    if(millis() > timer + 20){
        int value = map(n, 0, 500, 0, 256);
        if (value < 50) value = 0;

        temp = streamData[NUMBER_OF_LEDS-1];
        for(int i = NUMBER_OF_LEDS - 1; i > 0; i--){
            streamData[i] = streamData[i-1];
        }
        streamData[0] = value;

        for(int i = 0; i < NUMBER_OF_LEDS; i++){
            ledmaster.setLedColor(i, streamData[i] / 2, 0, streamData[i]);
        }
        timer = millis();
    }
}