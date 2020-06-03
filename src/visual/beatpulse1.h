#ifndef BEATPULSE1_H
#define BEATPULSE1_H
#include <Arduino.h>
#include "ledmaster.h"

extern Ledmaster ledmaster;

class Beatpulse1{

    public:
        Beatpulse1();
        void doVisual(int n, uint16_t beatLimit);
    private:
        uint32_t 
            timer,
            timer2,
            color;

};

#endif