#include "GenSinus.h"
#include <vector>
#include <iostream>
#include <fstream>

using namespace std;

const double PI = 3.14159265359;

void longWrite(long x, ofstream& file)
{
    file << static_cast<uint8_t>(x & 0xff);
    x /= 256;
    file << static_cast<uint8_t>(x & 0xff);
    x /= 256;
    file << static_cast<uint8_t>(x & 0xff);
    x /= 256;
    file << static_cast<uint8_t>(x & 0xff);
}

void integerWrite(uint32_t x, ofstream& file)
{
    file << static_cast<uint8_t>(x & 0xff);
    x /= 256;
    file << static_cast<uint8_t>(x & 0xff);
}

void GenSinus::SetSampleRate(long sRate)
{
    if (sRate > 0 && sRate <= 44100)
        this->sampleRate = sRate;
    else exit(0);
}
void GenSinus::SetWaveVolume(int wVol)
{
    if (wVol > 0 && wVol <= 32767)
        this->waveVolume = wVol;
    else exit(0);
}
void GenSinus::SetWaveFrequency(int wFreq)
{
    if (wFreq > 0 && wFreq <= sampleRate / 2)
        this->waveFrequency = wFreq;
    else exit(0);
}
void GenSinus::SetTime(int time)
{
    this->time = time;
}

void GenSinus::WriteHeader(ofstream& file)
{
    char chunk[4] = { 'R','I','F','F' }, format[4] = { 'W','A','V','E' },
        fmt[4] = { 'f','m','t',' ' }, data[4] = { 'd','a','t','a' };

    file.write(chunk, 4);
    longWrite(sampleRate * 2, file); //���
    file.write(format, 4);
    file.write(fmt, 4);
    longWrite(0x10, file); //������ ����� - 16(0�10) ��� ������� PCM
    integerWrite(1, file); //������ ����� - 1 �� ���������
    integerWrite(1, file); // ����� ������� (1 � ����, 2-������)
    longWrite(sampleRate, file); // ������� �������������
    longWrite(sampleRate * 2, file); // ������� ����� ���� � �������, ������������ ��� ����������� �����������
    integerWrite(2, file); // ������������ ������ � data-�����. 
    integerWrite(16, file); // �������� �������� (8 ���, 16 ���, ...)
    file.write(data, 4);
    longWrite(sampleRate * time * 2, file);
}

void GenSinus::Generation(ofstream& file)
{
    float period = sampleRate / waveFrequency / 2;

    _data = new short[sampleRate];

    for (int index = 0; index < sampleRate; index++) {
        _data[index] = waveVolume * sin(index * PI / period); //���������� sine-�����
    }

    for (int index = 0, el = 0; index < sampleRate * time; index++, el++)
    {
        if (el == sampleRate)
        {
            el = 0;
        }
        integerWrite(_data[el], file); // ���������� ������ � ����
    }
}

GenSinus::GenSinus()
{
    sampleRate = 44100;
    waveFrequency = 11025;
    waveVolume = 32765;
    time = 10;
}

GenSinus::~GenSinus()
{
    delete[] _data;
}