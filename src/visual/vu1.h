#ifndef VU1_H
#define VU1_H
#include <Arduino.h>
#include "ledmaster.h"

extern Ledmaster ledmaster;

class Vu1 : public Ledmaster{

    public:
        Vu1();
        void doVisual(int n);
    private:
        int
            vol[30],       // Collection of prior volume samples
            lvl = 10,
            minLvlAvg = 0,      // For dynamic adjustment of graph low & high
            maxLvlAvg = 600,
            wheelPos  = 0;

        byte
            peak      = 0,      // Used for falling dot
            dotCount  = 0,      // Frame counter for delaying dot-falling speed
            color     = 0,
            volCount  = 0;      // Frame counter for storing past volume data

        uint32_t 
            timer     = 0;

};

#endif