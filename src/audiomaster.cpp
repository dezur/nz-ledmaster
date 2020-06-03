#include "audiomaster.h"

Audiomaster::Audiomaster(){
    pinMode(AUDIO_INPUT, INPUT);
    ty = TYPE::LOWPASS;
    hz = 150.0;
    ts = 0.001;
    od = IIR::ORDER::OD3;

}

void Audiomaster::loop(){
    audioLevel = analogRead(AUDIO_INPUT);
    audioLevel = abs(audioLevel - 512 - DC_OFFSET);
}

uint16_t Audiomaster::getAudioLevel(){
    return audioLevel;
}

void Audiomaster::setBeatLimit(uint16_t limit){
    beatLimit = limit;
}

uint16_t Audiomaster::getBeatLimit(){
    return beatLimit;
}

void Audiomaster::init(bool doFlush) {
  if(doFlush) flush();
  f_err  = false;
  f_warn = false;

  switch ((uint8_t)ty) {
    case (uint8_t)TYPE::LOWPASS :
      initLowPass();
      break;
    case (uint8_t)TYPE::HIGHPASS :
      initHighPass();
      break;
  }
}

float_t Audiomaster::filterIn(float input) {
  if(f_err) return 0.0;

  switch ((uint8_t)ty) {
    case (uint8_t)TYPE::LOWPASS :
      return computeLowPass(input);
      break;
    case (uint8_t)TYPE::HIGHPASS :
      return computeHighPass(input);
      break;
    default:
      return input;
  }
}

void Audiomaster::flush() {
  for(uint8_t i=0; i<MAX_ORDER; i++) {
    u[i] = 0.0;
    y[i] = 0.0;
  }
}

// PRIVATE METHODS  * * * * * * * * * * * * * * * * * * * *

inline float_t Audiomaster::computeLowPass(float_t input) {
  for(uint8_t i=MAX_ORDER-1; i>0; i--) {
    y[i] = y[i-1];
    u[i] = u[i-1];
  }

  switch((uint8_t)od) {
    case (uint8_t)ORDER::OD1:
        y[0] = k1*y[1] + k0*input;
      break;
    case (uint8_t)ORDER::OD2:
        y[0] = k1*y[1] - k2*y[2] + (k0*input)/KM;
      break;
    case (uint8_t)ORDER::OD3:
        y[0] = k1*y[1] - k2*y[2] + k3*y[3] + (k0*input)/KM;
      break;
    case (uint8_t)ORDER::OD4:
        y[0] = k1*y[1] - k2*y[2] + k3*y[3] - k4*y[4] + (k0*input)/KM;
      break;
    default:
        y[0] = input;
      break;
  }
  return y[0];
}

inline float_t Audiomaster::computeHighPass(float_t input) {
  for(uint8_t i=MAX_ORDER-1; i>0; i--) {
    y[i] = y[i-1];
    u[i] = u[i-1];
  }
  u[0] = input; 

  switch((uint8_t)od) {
    case (uint8_t)ORDER::OD1:
        y[0] = k1*y[1] + j0*u[0] + j1*u[1];
      break;
    case (uint8_t)ORDER::OD2:
    case (uint8_t)ORDER::OD3:
    case (uint8_t)ORDER::OD4:
        y[0] = k1*y[1] + k2*y[2] + j0*u[0] + j1*u[1] + j2*u[2];
      break;
    default:
        y[0] = u[0];
      break;        
  }
  return y[0];
}


inline void  Audiomaster::initLowPass() {
  switch((uint8_t)od) {
    case (uint8_t)ORDER::OD1:
        a  = 2.0*PI*hz;
        k1 = exp(-a*ts);
        k0 = 1.0 - k1;
      break;
    case (uint8_t)ORDER::OD2:
        a  = -PI*hz*SQRT2;
        b  =  PI*hz*SQRT2;
        k2 = ap(exp(2.0*ts*a));
        k1 = ap(2.0*exp(a*ts)*cos(b*ts));
        k0 = ap(1.0*KM - k1*KM + k2*KM);
      break;
    case (uint8_t)ORDER::OD3:
        a  = -PI*hz;
        b  =  PI*hz*SQRT3;
        c  =  2.0*PI*hz;
        b3 = exp(-c*ts);
        b2 = exp(2.0*ts*a);
        b1 = 2.0*exp(a*ts)*cos(b*ts);
        k3 = ap(b2*b3);
        k2 = ap(b2 + b1*b3);
        k1 = ap(b1 + b3);
        k0 = ap(1.0*KM - b1*KM + b2*KM -b3*KM + b1*KM*b3 - b2*KM*b3);
      break;
    case (uint8_t)ORDER::OD4:
        a  = -0.3827*2.0*PI*hz;
        b  =  0.9238*2.0*PI*hz;
        c  = -0.9238*2.0*PI*hz;
        d  =  0.3827*2.0*PI*hz;
        b4 = exp(2.0*ts*c);
        b3 = 2.0*exp(c*ts)*cos(d*ts);
        b2 = exp(2.0*ts*a);
        b1 = 2.0*exp(a*ts)*cos(b*ts);
        k4 = ap(b2*b4);
        k3 = ap(b1*b4 + b2*b3);
        k2 = ap(b4 + b1*b3 + b2);
        k1 = ap(b1 + b3);
        k0 = ap(1.0*KM - k1*KM + k2*KM - k3*KM + k4*KM);
      break;
  }
}

// a0..aN Terms are the TF's denominator coeffs; 
// b0..bN Terms are the TF's numerator coeffs;
// k0..kN Terms multiply the diff. equation state terms (y) with 0 to N delays, respectively 
// j0..jN Terms multiply the diff. equation input terms (u) with 0 to N delays, respectively
inline void  Audiomaster::initHighPass() {
  // Bilinear transformation
  float_t k  = 2.0/ts;
  float_t w0 = 2.0*PI*hz;

  switch((uint8_t)od) {
      case (uint8_t)ORDER::OD1:
          // TF Terms
          b0 =  k;
          b1 = -k;
          a0 = (w0 + k);
          a1 = (w0 - k);
          // Diff equation terms
          j0 =  b0/a0;
          j1 =  b1/a0;
          k1 = -a1/a0;
        break;
      case (uint8_t)ORDER::OD2:
      case (uint8_t)ORDER::OD3:
      case (uint8_t)ORDER::OD4:
          float_t w0sq = pow(w0, 2.0);
          float_t ksq  = pow(k,  2.0);
          // TF Terms
          b0 = ksq;
          b1 = -2.0*ksq;
          b2 = ksq;
          a0 = w0sq + k*w0 + ksq;
          a1 = 2.0*w0sq - 2.0*ksq;
          a2 = w0sq - k*w0 + ksq;
          // Diff equation terms
          j0 = b0/a0;
          j1 = b1/a0;
          j2 = b2/a0;
          k1 = -a1/a0;
          k2 = -a2/a0;
        break;
      }
}

float_t Audiomaster::ap(float_t p) {
  f_err  = f_err  | (abs(p) <= EPSILON );
  f_warn = f_warn | (abs(p) <= WEPSILON);
  return (f_err) ? 0.0 : p;
}
