#include "fabricMaths.hpp"
#include "Grain.hpp"
#include <iostream>
#include <math.h>
#include <algorithm>

Grain::Grain() : 
     m_playing(false)
    ,m_startTimeFrameOffset(0)
    ,m_length(0)
    ,m_startTimeBuffer(0)
    ,m_age(0)
    ,m_positionInAudioBuffer(0)
    ,m_speed(1.f)
{

}

Grain::~Grain()
{

}

void Grain::process(float** outputs, float** st_audioBuffer, int st_audioBufferSize, int subdivStart, int frames)
{
    //copy the data from member variable, to local variable to improve performance
    float startTimeBuffer = m_startTimeBuffer;

    float *leftOutput = outputs[0] + subdivStart;
    float *rightOutput = outputs[1] + subdivStart;
 
    const float *leftBuffer = st_audioBuffer[0];
    const float *rightBuffer = st_audioBuffer[1];
 
    //player should guarantee this
    assert(m_startTimeFrameOffset < frames);
 
    //offset output by the starting delay
    leftOutput += m_startTimeFrameOffset;
    rightOutput += m_startTimeFrameOffset;
    frames -= m_startTimeFrameOffset;
    m_startTimeFrameOffset = 0;
    
    //do not output past the grain's lifetime
    frames = std::min(frames, (int)std::ceil(m_age));

    for (int framePos = 0; framePos < frames; ++framePos)
    {
        float i = (m_length - m_age) / m_length;
        float window = fabricMaths::tukeyWindow(i, m_sides, m_tilt);

        // lerp for accessing the stereo audio buffer
        int startTimeBufferLerp = ((int)std::floor(startTimeBuffer + 1)) % st_audioBufferSize;
        float fraction = startTimeBuffer - (int)startTimeBuffer;
        
        float left = fabricMaths::lerp(leftBuffer[(int)startTimeBuffer % st_audioBufferSize], leftBuffer[startTimeBufferLerp], fraction);
        float right = fabricMaths::lerp(rightBuffer[(int)startTimeBuffer % st_audioBufferSize], rightBuffer[startTimeBufferLerp], fraction);

        leftOutput[framePos]  += left  * window;
        rightOutput[framePos] += right * window;

        startTimeBuffer += m_speed;
        startTimeBuffer > st_audioBufferSize ? st_audioBufferSize : startTimeBuffer;
        startTimeBuffer < 0 ? 0 : startTimeBuffer;
        m_age -= m_speed;
    }
    m_startTimeBuffer = startTimeBuffer;
}