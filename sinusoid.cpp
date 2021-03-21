#include <math.h> 
#include <fstream>
#include <iostream>

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

int main()
{
    setlocale(LC_ALL, "Rus");

    char chunk[4] = { 'R','I','F','F' }, format[4] = { 'W','A','V','E' },
        fmt[4] = { 'f','m','t',' ' }, data[4] = { 'd','a','t','a' };

    ofstream file("Test.pcm", ios::trunc | ios::binary);

    unsigned long sampleRate, waveVolume, time;
    short frameSize = (short)(16 / 8);
    float waveFrequency;

    cout << "Введите частоту дискретизации: "; cin >> sampleRate;
    cout << "Введите частоту тона: "; cin >> waveFrequency;
    cout << "Введите громкость тона: "; cin >> waveVolume; //0...65536
    cout << "Введите время: "; cin >> time;

    file.write(chunk, 4);
    longWrite(sampleRate * frameSize, file); //вес - 36 * количество байт в области данных
    file.write(format, 4);
    file.write(fmt, 4);
    longWrite(0x10, file); //размер чанка - 16(0х10) для формата PCM
    integerWrite((short)1, file); //формат аудио - 1 по умолчанию
    integerWrite((short)1, file); // число каналов (1 — моно, 2-стерео)
    longWrite(sampleRate, file); // частота дискретизации
    longWrite(sampleRate * frameSize, file); // среднее число байт в секунду, используется для эффективной буферизации. Для PCM вычисляется по формуле: (nChannels*nSamplesPerSec*nAvgBytesPerSec)/8.
    integerWrite(frameSize, file); // выравнивание данных в data-чанке. Для PCM вычисляется по формуле: (nChannels*nAvgBytesPerSec)/8.
    integerWrite((short)16, file); // точность звучания (8 бит, 16 бит, ...)
    file.write(data, 4);
    longWrite(sampleRate * frameSize * time, file);

    float period = sampleRate / waveFrequency / 2;

    short* _data = new short[sampleRate];

    for (int index = 0; index < sampleRate; index++) {
        _data[index] = waveVolume * sin(index * PI / period); //вычисление sine-волны
    }

    for (int index = 0, el = 0; index < sampleRate * time; index++, el++)
    {
        if (el == sampleRate)
        {
            el = 0;
        }
        integerWrite(_data[el], file); // записываем данные в файл
    }

    file.close();
}
