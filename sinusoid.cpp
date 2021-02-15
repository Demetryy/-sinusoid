#include "windows.h"
#include "mmsystem.h"
#include <math.h> 
#include <fstream>
#include <iostream>

using namespace std;

const double PI = 3.14159265359;

static double Sine(int index, double frequency) {
    return sin(frequency * index);
}

void InputOfSignatures(char* arr, ofstream& file)
{
    for (int i = 0; i < 4; i++)
    {
        file << arr[i];
    }
}

void longWrite(long x, ofstream& file)
{
    int x1 = x % 256;
    x -= x1;
    x /= 256;
    int x2 = x % 256;
    x = x - x2;
    x = x / 256;
    int x3 = x % 256;
    x = x - x1;
    x = x / 256;
    int x4 = x;

    unsigned char a = (unsigned char)x1;
    unsigned char b = (unsigned char)x2;
    unsigned char c = (unsigned char)x3;
    unsigned char d = (unsigned char)x4;

    file << a << b << c << d;
}

void integerWrite(unsigned int x, ofstream& file)
{
    int x1 = x % 256;
    x -= x1;
    x /= 256;

    unsigned char a = (unsigned char)x1;
    unsigned char b = (unsigned char)x;

    file << a << b;
}


int main()
{
    setlocale(LC_ALL, "Rus");

    char chunk[4] = { 'R','I','F','F' }, format[4] = { 'W','A','V','E' },
        fmt[4] = { 'f','m','t',' ' }, data[4] = { 'd','a','t','a' };

    ofstream file("Test.pcm", ios::trunc | ios::binary);

    WAVEFORMAT pcmObj;
    short frameSize = (short)(16 / 8);

    cout << "Введите частоту дискретизации: "; cin >> pcmObj.nSamplesPerSec;
    cout << "Введите число каналов: "; cin >> pcmObj.nChannels;

    pcmObj.nAvgBytesPerSec = pcmObj.nSamplesPerSec * 16 / 8;

    InputOfSignatures(chunk, file); //"RIFF"

    longWrite(220000, file); //вес - 36 * количество байт в области данных


    InputOfSignatures(format, file); //"WAVE"
    InputOfSignatures(fmt, file); //"fmt "

    longWrite(0x10, file); //размер чанка - 16(0х10) для формата PCM
    integerWrite(1, file); //формат аудио - 1 по умолчанию
    integerWrite(pcmObj.nChannels, file); // число каналов (1 — моно, 2-стерео)
    longWrite(pcmObj.nSamplesPerSec, file); // частота дискретизации
    longWrite(pcmObj.nAvgBytesPerSec, file); // среднее число байт в секунду, используется для эффективной буферизации. Для PCM вычисляется по формуле: (nChannels*nSamplesPerSec*nAvgBytesPerSec)/8.
    integerWrite(2, file); // выравнивание данных в data-чанке. Для PCM вычисляется по формуле: (nChannels*nAvgBytesPerSec)/8.
    integerWrite(16, file); // точность звучания (8 бит, 16 бит, ...)

    InputOfSignatures(data, file);

    int Time = 20;
    long DataLeng = pcmObj.nSamplesPerSec * Time;
    longWrite(DataLeng, file);


    short* _data = new short[pcmObj.nSamplesPerSec];
    double frequency = PI * 2 * 440.0 / pcmObj.nSamplesPerSec;

    for (int sec = 0; sec < Time; sec++) {

        for (int index = 0; index < pcmObj.nSamplesPerSec; index++) {
            _data[index] = (short)(Sine(index, frequency) * SHRT_MAX); // Приводим уровень к амплитуде от 32767 до -32767.
            integerWrite(_data[index], file); // записываем данные в файл
        }
    }

    file.close();
}