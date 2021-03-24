#include "GenSinus.h"
#include <fstream>
#include <cmath>

using namespace std;

const float PI = 3.14159265;

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

/*
void GenSinus::Initialization(unsigned long sampleRate, unsigned long waveFrequency, unsigned long waveVolume, unsigned long time)
{
    this->sampleRate = sampleRate;
    this->waveFrequency = waveFrequency;
    this->waveVolume = waveVolume;
    this->time = time;
}
*/

void GenSinus::WriteHeader(ofstream &file, unsigned long sampleRate, unsigned long time)
{
    short frameSize = (short)(16 / 8);
    char chunk[4] = { 'R','I','F','F' }, format[4] = { 'W','A','V','E' },
        fmt[4] = { 'f','m','t',' ' }, data[4] = { 'd','a','t','a' };

    file.write(chunk, 4);
    longWrite(sampleRate * frameSize, file); //��� - 36 * ���������� ���� � ������� ������
    file.write(format, 4);
    file.write(fmt, 4);
    longWrite(0x10, file); //������ ����� - 16(0�10) ��� ������� PCM
    integerWrite((short)1, file); //������ ����� - 1 �� ���������
    integerWrite((short)1, file); // ����� ������� (1 � ����, 2-������)
    longWrite(sampleRate, file); // ������� �������������
    longWrite(sampleRate * frameSize, file); // ������� ����� ���� � �������, ������������ ��� ����������� �����������. ��� PCM ����������� �� �������: (nChannels*nSamplesPerSec*nAvgBytesPerSec)/8.
    integerWrite(frameSize, file); // ������������ ������ � data-�����. ��� PCM ����������� �� �������: (nChannels*nAvgBytesPerSec)/8.
    integerWrite((short)16, file); // �������� �������� (8 ���, 16 ���, ...)
    file.write(data, 4);
    longWrite(sampleRate * frameSize * time, file);
}

void GenSinus::Generation(ofstream &file, unsigned long sampleRate, unsigned long waveFrequency, unsigned long waveVolume, unsigned long time)
{
    double period = sampleRate / waveFrequency / 2;

    float* _data = new float[sampleRate]; //���� short

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
