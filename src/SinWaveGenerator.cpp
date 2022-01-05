
#include "SinWaveGenerator.h"



void SinWaveGenerator::GetSamples()
{
    float full_wave_samples = AES.getSampleRate() / Frequency;
    float step_per_sample = M_TWOPI / full_wave_samples;
    // fill the buffer with data from the file wrapping around if necessary
    for (uint8_t i = 0; i < NUM_FRAMES_TO_SEND; i++)
    {
        int32_t Val  = Magnitude * sin(m_current_position);
        AES.WriteFrame(i,AES.x, Val);
        AES.WriteFrame(i, AES.y, Val);
        
        m_current_position += step_per_sample;
        // wrap around to maintain numerical stability
        if (m_current_position > M_TWOPI)
        {
            m_current_position -= M_TWOPI;
        }
    }
}

SinWaveGenerator Generator;



