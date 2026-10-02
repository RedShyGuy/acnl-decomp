#include "nn/snd/CTR/snd_ThreadManager.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00461F60 | nintendogs:callgraph [tier A]
void nn::snd::CTR::ThreadManager::GetInstance()
{
}

// 0x00462008 | nintendogs:callgraph [tier A]
void nn::snd::CTR::ThreadManager::StartSoundThread(void(*)(unsigned), unsigned, unsigned, unsigned, int, int)
{
}

// 0x0046212C | nintendogs:bytes [tier A]
void nn::snd::CTR::ThreadManager::StartSoundThread(const nn::snd::CTR::ThreadParameter*, void(*)(unsigned), unsigned, const nn::snd::CTR::ThreadParameter*, void(*)(unsigned), unsigned, int)
{
}

// 0x004621F4 | nintendogs:callgraph [tier A]
void nn::snd::CTR::ThreadManager::FinalizeSoundThread()
{
}

// 0x004622F0 | fefates:bytes [tier B]
void nn::snd::CTR::ThreadManager::SoundThreadFuncImpl(unsigned int)
{
}

// 0x00462474 | nintendogs:bytes [tier A]
void nn::snd::CTR::ThreadManager::StartUserSoundThread(unsigned, unsigned, int)
{
}

// 0x00462594 | nintendogs:bytes [tier A]
nn::snd::CTR::ThreadManager::~ThreadManager()
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
