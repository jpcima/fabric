#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "Grain.hpp"

#define MAX_GRAINS 128

struct GrainPlayerControl
{
    float speed = 1.f;
    float density = 0.01f;
    float length = 0.01f;
    float spray = 0.01f;
    float sides;
    float playHeadPos = 0.f;
    float sampleRate = 44100.f;
};

class GrainPlayer
{
public:
    GrainPlayer();
    ~GrainPlayer();
    void generate(float** outputs, float** st_audioBuffer, int bufferSize, uint32_t frames);
    GrainPlayerControl controls;

    Grain grainArray[MAX_GRAINS];
private:
    uint32_t _frames;
    int _nextGrainTime = {0};
    int _bufferSize = {0};
    void addGrain(int currentFrame);
};