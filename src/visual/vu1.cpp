#include "vu1.h"

Vu1::Vu1(){
    
}

void Vu1::doVisual(int n){
    uint8_t  i;
    int      height;

    lvl = ((lvl * 7) + n) >> 3; 
    height = map(lvl, minLvlAvg, maxLvlAvg, 0, 60);
    
    if(height < 0L)       height = 0;
    else if(height > 60) height = 60;
    if(height > peak)     peak   = height;

    for(i=0; i<60; i++) {
        if(i <= height)
        ledmaster.setLedColor(i, ledmaster.Wheel(color));
    }
    
    if(peak > 0 && peak <= 60-1) 
        ledmaster.ledFadeOut(10);
    
    if(++dotCount >= 200) {
        if(peak > 0) peak--;
            dotCount = 0;
    }
    if(millis() > timer +100){
        color++;
        timer = millis();
    }
}