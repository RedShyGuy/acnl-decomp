#include "nn/snd/CTR/CTR_Api.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x001409E8 | nintendogs:callgraph [tier A]
void FlushDataCache(unsigned, unsigned)
{
}

// 0x00460794 | nintendogs:callgraph [tier A]
void AllocVoice(int, void(*)(nn::snd::CTR::Voice*, unsigned), unsigned)
{
}

// 0x00460C44 | nintendogs:bytes [tier A]
void Initialize()
{
}

// 0x004621BC | nintendogs:callgraph [tier A]
void GetSoundThreadTick()
{
}

// 0x004621E4 | nintendogs:callgraph [tier A]
void FinalizeSoundThread()
{
}

// 0x00462600 | fefates:bytes [tier B]
void WaitForDspSync(nn::os::Tick*)
{
}

// 0x00462794 | fefates:bytes [tier B]
void WaitForDspSync()
{
}

// 0x004628D8 | nintendogs:bytes [tier A]
void DecodeAdpcmData(const unsigned char*, short*, const nn::snd::CTR::AdpcmParam&, nn::snd::CTR::AdpcmContext&, int)
{
}

// 0x00462DFC | nintendogs:bytes [tier A]
void StartSoundThread(const nn::snd::CTR::ThreadParameter*, void(*)(unsigned), unsigned, const nn::snd::CTR::ThreadParameter*, void(*)(unsigned), unsigned, int)
{
}

// 0x004633B4 | nintendogs:callgraph [tier A]
void GetHeadphoneStatus()
{
}

// 0x004633D0 | fefates:bytes [tier B]
void SendParameterToDsp()
{
}

// 0x00463470 | nintendogs:callseq-callee [tier A]
void SetAuxReturnVolume(nn::snd::CTR::AuxBusId, float)
{
}

// 0x004634A8 | fefates:bytes [tier B]
void UserSoundThreadFunc(unsigned int)
{
}

// 0x00463560 | nintendogs:bytes [tier A]
void InitializeWaveBuffer(nn::snd::CTR::WaveBuffer*)
{
}

// 0x004635B0 | nintendogs:bytes [tier A]
void OrderToWaitForFinalize()
{
}

// 0x004635EC | nintendogs:bytes [tier A]
void Sleep()
{
}

// 0x0046538C | nintendogs:bytes [tier A]
void WakeUp()
{
}

// 0x004658E4 | nintendogs:bytes [tier A]
void Finalize()
{
}

// 0x0046633C | nintendogs:callgraph [tier A]
void FreeVoice(nn::snd::CTR::Voice*)
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
