#pragma once
#include <fstream>

class GenSinus
{
public:
    void WriteHeader(std::ofstream& file);
    void Generation(std::ofstream& file);
    void SetSampleRate(long sRate), SetWaveVolume(int wVol),
        SetWaveFrequency(int wFreq), SetTime(int time);
    GenSinus(long sampleRate, int waveFrequency, int waveVolume, int time);
    ~GenSinus();
private:
    long sampleRate;
    int waveFrequency, waveVolume, time;
    short* _data;
};
