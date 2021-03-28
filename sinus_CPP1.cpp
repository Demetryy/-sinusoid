#include "GenSinus.h"
#include <math.h> 
#include <fstream>
#include <iostream>
#include <functional>   
#include <map>          
#include <memory>      
#include <string>       
#include <sstream>      
#include <string_view>  
#include <variant>      
#include <vector>       
using namespace std;

const double PI = 3.14159265359;

template <class Opts>
struct CmdOpts : Opts
{
    using MyProp = variant<long Opts::*, int Opts::*>;
    using MyArg = pair<string, MyProp>;

    ~CmdOpts() = default;

    Opts parse(int argc, char* argv[])
    {
        vector<string_view> vargv(argv, argv + argc);
        for (int idx = 0; idx < argc; ++idx)
            for (auto& cbk : callbacks)
                cbk.second(idx, vargv);

        return static_cast<Opts>(*this);
    }

    static unique_ptr<CmdOpts> Create(std::initializer_list<MyArg> args)
    {
        auto cmdOpts = unique_ptr<CmdOpts>(new CmdOpts());
        for (auto arg : args) cmdOpts->register_callback(arg);
        return cmdOpts;
    }

private:
    using callback_t = function<void(int, const vector<string_view>&)>;
    map<string, callback_t> callbacks;

    CmdOpts() = default;
    CmdOpts(const CmdOpts&) = delete;
    CmdOpts(CmdOpts&&) = delete;
    CmdOpts& operator=(const CmdOpts&) = delete;
    CmdOpts& operator=(CmdOpts&&) = delete;

    auto register_callback(string name, MyProp prop)
    {
        callbacks[name] = [this, name, prop](int idx, const vector<string_view>& argv)
        {
            if (argv[idx] == name)
            {
                visit(
                    [this, idx, &argv](auto&& arg)
                    {
                        if (idx < argv.size() - 1)
                        {
                            stringstream value;
                            value << argv[idx + 1];
                            value >> this->*arg;
                        }
                    },
                    prop);
            }
        };
    };

    auto register_callback(MyArg p) { return register_callback(p.first, p.second); }
};

int main(int argc, char* argv[])
{
    setlocale(LC_ALL, "Rus");

    ofstream file("Test.pcm", ios::trunc | ios::binary);

    GenSinus obj;

    struct Wave
    {
        long sampleRate;
        int waveFrequency, waveVolume, time;
    };

    auto parser = CmdOpts<Wave>::Create({
        {"--sampleRate", &Wave::sampleRate},
        {"--waveFrequency", &Wave::waveFrequency},
        {"--waveVolume", &Wave::waveVolume},
        {"--time", &Wave::time} });

    auto wave = parser->parse(argc, argv);

    obj.SetSampleRate(wave.sampleRate);
    obj.SetWaveFrequency(wave.waveFrequency);//0...32767
    obj.SetWaveVolume(wave.waveVolume);
    obj.SetTime(wave.time);

    obj.WriteHeader(file);
    obj.Generation(file);

    obj.~GenSinus();

    file.close();
}