#include "beatpulse1.h"

Beatpulse1::Beatpulse1(){
    
}

void Beatpulse1::doVisual(int n){
    int      value;
    value = n;
    
    /*
    if(value > maxLevelAvg){
        if(millis() > timer + 300){
            hinyje = random (0, 50);
            
            valuehe = 250;
            for(int i = hinyje; i < hinyje + 10; i++){
                ledmaster.setLedColor(i, ledmaster.Wheel(color++));
            }
            timerja = millis();
        }
    }
    ledmaster.ledFadeOut(1);
    */
   
    for(int i = 0; i < NUMBER_OF_LEDS; i++){
        if(value > sensitivity)
            ledmaster.setLedColor(i, 0, 0, 255);
        else
            if(millis() > timer + 5){
                ledmaster.ledFadeOut(5);
                timer = millis();
            }
    }
}