#ifndef BEATPULSE1_H
#define BEATPULSE1_H
#include <Arduino.h>
#include "ledmaster.h"

#define TOP 60
#define SAMPLES 60

extern Ledmaster ledmaster;

class Beatpulse1{

    public:
        Beatpulse1();
        void doVisual(int n);
    private:
        int
            sensitivity = 300,
            valuehe = 0;

        byte
            color = 0;

        uint32_t timer;

};

#endif