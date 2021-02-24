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
    file.write(arr,4);
}

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
        x /= 256;
        file << static_cast<uint8_t>(x & 0xff);
        x /= 256;
        file << static_cast<uint8_t>(x & 0xff);
}


int main()
{
    setlocale(LC_ALL, "Rus");

    char chunk[4] = { 'R','I','F','F' }, format[4] = { 'W','A','V','E' },
        fmt[4] = { 'f','m','t',' ' }, data[4] = { 'd','a','t','a' };

    ofstream file("Test.pcm", ios::trunc | ios::binary);

    WAVEFORMAT pcmObj;
    short frameSize = (short)(16 / 8);
    int time;

    cout << "Введите частоту дискретизации: "; cin >> pcmObj.nSamplesPerSec;
    cout << "Введите число каналов: "; cin >> pcmObj.nChannels;
    cout << "Введите время: "; cin >> time;

    pcmObj.nAvgBytesPerSec = pcmObj.nSamplesPerSec * 16 / 8;

    InputOfSignatures(chunk, file); //"RIFF"

    longWrite(pcmObj.nSamplesPerSec * frameSize, file); //вес - 36 * количество байт в области данных


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

    long DataLeng = pcmObj.nSamplesPerSec * time;
    longWrite(DataLeng, file);


    short* _data = new short[pcmObj.nSamplesPerSec];
    double frequency = PI * 2 * 440.0 / pcmObj.nSamplesPerSec;

    for (int sec = 0; sec < time*2; sec++) {

        for (int index = 0; index < pcmObj.nSamplesPerSec; index++) {
            _data[index] = (short)(Sine(index, frequency) * SHRT_MAX); // Приводим уровень к амплитуде от 32767 до -32767.
            integerWrite(_data[index], file); // записываем данные в файл
        }
    }

    file.close();
}
