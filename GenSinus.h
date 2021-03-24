#pragma once
#include <string>
#include <fstream>

class GenSinus
{
public:
    void WriteHeader(ofstream& file, unsigned long sampleRate, unsigned long time);
    void Generation(ofstream& file, unsigned long sampleRate, unsigned long waveFrequency, unsigned long waveVolume, unsigned long time);
    //void Initialization(unsigned long sampleRate, unsigned long waveFrequency, unsigned long waveVolume, unsigned long time)
    unsigned long sampleRate, waveVolume, waveFrequency, time;
};
