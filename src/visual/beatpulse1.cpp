#include "beatpulse1.h"

Beatpulse1::Beatpulse1(){
    
}

void Beatpulse1::doVisual(int n, uint16_t beatLimit){
    for(int i = 0; i < NUMBER_OF_LEDS; i++){
        if(n > beatLimit){
            ledmaster.setLedColor(i, ledmaster.Wheel(color));
        }
        else if(millis() > timer + 5){
            ledmaster.ledFadeOut(3);
            timer = millis();
        }
    }
    if(millis() > timer2 + 50){
        color++;
        timer2 = millis();
    }
}