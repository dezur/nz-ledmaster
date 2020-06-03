#include "vu1.h"

Vu1::Vu1(){
    
}

void Vu1::doVisual(int n){
    uint8_t  i;
    int      height;
    uint16_t minLvl, maxLvl;

    lvl = ((lvl * 7) + n) >> 3; 
    height = map(lvl, 0, maxLvlAvg, 0, 60);
    
    if(height < 0L)       height = 0;
    else if(height > 60) height = 60;
    if(height > peak)     peak   = height;

    for(i=0; i<60; i++) {
        if(i <= height)
        ledmaster.setLedColor(i, ledmaster.Wheel(color));
    }
    
    if(peak > 0 && peak <= TOP) 
        ledmaster.ledFadeOut(5);
    
    if(++dotCount >= 200) {
        if(peak > 0) peak--;
            dotCount = 0;
    }
    if(millis() > timer +100){
        color++;
        timer = millis();
    }

    vol[volCount] = n;
    if(++volCount >= SAMPLES) volCount = 0;
    minLvl = maxLvl = vol[0];

    for(i=1; i<SAMPLES; i++) {
        if(vol[i] < minLvl)
            minLvl = vol[i];
        else if(vol[i] > maxLvl) 
            maxLvl = vol[i];
    }
    if((maxLvl - minLvl) < TOP) 
        maxLvl = minLvl + TOP;

    maxLvlAvg = ((maxLvlAvg * 61 + maxLvl) >> 6);
    if(maxLvlAvg < 200) maxLvlAvg = 200;
}

uint16_t Vu1::getMaxLevel(){
    return maxLvlAvg;
}