#ifndef AUDIOMASTER_H
#define AUDIOMASTER_H
#include <Arduino.h>
#include "filters_defs.h"

#define AUDIO_INPUT 39
#define DC_OFFSET -500

using namespace IIR;

class Audiomaster{

    public:
        Audiomaster();
        void loop();
        uint16_t getAudioLevel();

        float_t filterIn(float_t input);

        // FILTER STUFF
        void flush();
        void init(bool doFlush=true);

        void setSamplingTime(float_t ts_, bool doFlush=true) { ts = ts_; init(doFlush); }
        void setCutoffFreqHZ(float_t hz_, bool doFlush=true) { hz = hz_; init(doFlush); }
        void setOrder(ORDER od_, bool doFlush=true)          { od = od_; init(doFlush); }

        bool isInErrorState() { return f_err;  }
        bool isInWarnState()  { return f_warn; }

    private:
        uint16_t audioLevel;

        // FILTER STUFF
        float_t ts;
        float_t hz;
        ORDER od;
        TYPE  ty;

        // Helper variables during coefficient calcutions
        float_t a, b, c, d, e;
        // Filter coefficients
        float_t b0, b1, b2, b3, b4, a0, a1, a2;
        // Difference equation terms 
        float_t k0, k1, k2, k3, k4, k5;
        float_t j0, j1, j2;
        // Filter buffer 
        float_t y[MAX_ORDER], u[MAX_ORDER];

        bool f_err, f_warn; ///< Numerical error or warning; only relevant for 8-bit micros

        float_t ap(float_t p); ///< Assert Parameter

        inline float_t computeLowPass(float_t input);
        inline float_t computeHighPass(float_t input);

        /** \brief Computes the discrete coefficients for a Butterworth low-pass filter via pole-zero matching. 
         *  Up to order 4. 
         */ 
        inline void  initLowPass();

        /** \brief Computes the discrete coefficients for a Butterworth high-pass filter via Bilinear-Transformation.
         *  Up to order 2 (I was lazy).
         */
        inline void  initHighPass();
};

#endif