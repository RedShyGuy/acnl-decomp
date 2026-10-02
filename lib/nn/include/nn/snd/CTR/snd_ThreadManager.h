#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class ThreadManager
{
public:
    void GetInstance(); // 0x00461F60 | nintendogs:callgraph [tier A]
    void StartSoundThread(void(*)(unsigned), unsigned, unsigned, unsigned, int, int); // 0x00462008 | nintendogs:callgraph [tier A]
    void StartSoundThread(const nn::snd::CTR::ThreadParameter*, void(*)(unsigned), unsigned, const nn::snd::CTR::ThreadParameter*, void(*)(unsigned), unsigned, int); // 0x0046212C | nintendogs:bytes [tier A]
    void FinalizeSoundThread(); // 0x004621F4 | nintendogs:callgraph [tier A]
    void SoundThreadFuncImpl(unsigned int); // 0x004622F0 | fefates:bytes [tier B]
    void StartUserSoundThread(unsigned, unsigned, int); // 0x00462474 | nintendogs:bytes [tier A]
    ~ThreadManager(); // 0x00462594 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn
