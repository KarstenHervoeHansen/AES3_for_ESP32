#ifndef __sin_wave_generator_h__
#define __sin_wave_generator_h__


#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include "AES.h"
#include "math.h"


class SinWaveGenerator
{
private:
    uint32_t Frequency = 1000;
    uint32_t Magnitude = 0x3FFFFF;
    float m_current_position = 0;

public:
    void GetSamples();
};

extern SinWaveGenerator Generator;

#endif