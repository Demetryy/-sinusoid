#include "GenSinus.h"
#include <iostream>
#include <fstream>
#include <string>
#include <vector>       
using namespace std;

const double PI = 3.14159265359;

int main(int argc, const char** argv)
{
    // parse args
    cout << "commands: \n"
        << "-s 44100 -f 11025 -t 10 -v 32767\n";
    vector<string> args(argv, argv + argc);
    for (int i = 0; i < argc; i++)
        cout << "argument [" << i << "] : " << args[i] << endl;
    vector<string> commands{ "-fn","-s","-f","-t","-v" };

    // default values
    ofstream file("Test.pcm", ios::trunc | ios::binary);
    long sampleRate = 44100;
    int waveFrequency = 11025;
    int time = 10;
    int waveVolume = 32767;

    if (argc != 1) // если мы имеем аргументы
    {
        string buf;
        for (auto& arg : args)
            buf += arg;
        int countCmd = count(buf.begin(), buf.end(), '-');
        int countArgs = argc - countCmd - 1;
        if (countCmd == countArgs && countCmd <= static_cast<int>(commands.size()))
        {
            // continue parse
            // parse from 1 index
            pair<string, string> keys;
            for (int i = 1; i < (countCmd + countArgs); i += 2)
            {
                // составляем пары (ключ + аргумент)
                keys = make_pair(args.at(i), args.at(i + 1));
                auto it = find(commands.begin(), commands.end(), keys.first);
                if (it != commands.end())
                {
                    int foundIndex = it - commands.begin();
                    switch (foundIndex)
                    {
                    case 0:
                        sampleRate = stof(keys.second);
                        break;
                    case 1:
                        waveFrequency = stof(keys.second);
                        break;
                    case 2:
                        time = stof(keys.second);
                        break;
                    case 3:
                        waveVolume = stof(keys.second);
                        break;
                    }
                }
            }
        }
    }

       cout << "Sample rate: " << sampleRate << endl
        << "Frequency: " << waveFrequency << endl
        << "Duration: " << time << endl
        << "Volume: " << waveVolume << endl;

    GenSinus objWave(sampleRate, waveFrequency, waveVolume, time);
    objWave.WriteHeader(file);
    objWave.Generation(file);

    objWave.~GenSinus();

    system("pause");
}
