#include "GenSinus.h"
#include <math.h> 
#include <fstream>
#include <iostream>

using namespace std;

const double PI = 3.14159265359;

int main()
{
    setlocale(LC_ALL, "Rus");

    ofstream file("Test.pcm", ios::trunc | ios::binary);
    GenSinus obj;
    
    cout << "Введите частоту дискретизации: "; cin >> obj.sampleRate;
    cout << "Введите частоту тона: "; cin >> obj.waveFrequency;
    cout << "Введите громкость тона: "; cin >> obj.waveVolume; //0...65536
    cout << "Введите время: "; cin >> obj.time;

    obj.WriteHeader(file, obj.sampleRate, obj.time);

    obj.Generation(file, obj.sampleRate, obj.waveFrequency, obj.waveVolume, obj.time);

    file.close();
}