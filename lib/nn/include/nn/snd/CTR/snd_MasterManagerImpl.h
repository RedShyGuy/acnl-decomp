#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class MasterManagerImpl
{
public:
    void Initialize(); // 0x00462E3C | nintendogs:bytes [tier A]
    void AuxUserCallback(nn::snd::CTR::AuxBusId, unsigned); // 0x00462E64 | nintendogs:bytes [tier A]
    void InitializeParam(); // 0x00462EE4 | nintendogs:callgraph [tier A]
    void SetMasterVolume(float); // 0x004630CC | nintendogs:bytes [tier A]
    void ForceUpdateParams(); // 0x004630F4 | nintendogs:bytes [tier A]
    void RegisterAuxCallback(nn::snd::CTR::AuxBusId, void(*)(nn::snd::CTR::AuxBusData*, int, unsigned), unsigned); // 0x00463268 | nintendogs:bytes [tier A]
    void SetOutputBufferCount(int); // 0x004632D4 | nintendogs:bytes [tier A]
    void EnableFx(nn::snd::CTR::AuxBusId, bool); // 0x00463304 | nintendogs:bytes [tier A]
    void Finalize(); // 0x00463368 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn
