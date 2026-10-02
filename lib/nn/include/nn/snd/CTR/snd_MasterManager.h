#pragma once

#include "decomp.h"

namespace nn {
namespace snd {
namespace CTR {
class MasterManager
{
public:
    void Initialize(); // 0x00461A48 | nintendogs:bytes [tier A]
    void ClearEffect(nn::snd::CTR::AuxBusId); // 0x00461B28 | nintendogs:bytes [tier A]
    void GetDspCycles(); // 0x00461B88 | nintendogs:bytes [tier A]
    void ExecuteEffect(nn::snd::CTR::AuxBusId, unsigned); // 0x00461C08 | nintendogs:bytes [tier A]
    void GetAuxCallback(nn::snd::CTR::AuxBusId, void(**)(nn::snd::CTR::AuxBusData*, int, unsigned), unsigned*); // 0x00461C7C | nintendogs:bytes [tier A]
    void AuxUserCallback(nn::snd::CTR::AuxBusId, unsigned); // 0x00461C94 | nintendogs:bytes [tier A]
    void SetMasterVolume(float); // 0x00461CB0 | nintendogs:bytes [tier A]
    void ClearAuxCallback(nn::snd::CTR::AuxBusId); // 0x00461CD0 | nintendogs:bytes [tier A]
    void RegisterAuxCallback(nn::snd::CTR::AuxBusId, void(*)(nn::snd::CTR::AuxBusData*, int, unsigned), unsigned); // 0x00461D40 | nintendogs:bytes [tier A]
    void SetIsHeadsetConnected(bool); // 0x00461D64 | nintendogs:callgraph [tier A]
    void UpdateDroppedSoundFrameCount(); // 0x00461D74 | nintendogs:bytes [tier A]
    void Finalize(); // 0x00461DA0 | nintendogs:bytes [tier A]
    void SetEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::FxDelay*); // 0x00461DD4 | nintendogs:bytes [tier A]
    void SetEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::FxReverb*); // 0x00461E78 | nintendogs:bytes [tier A]
};
} // namespace CTR
} // namespace snd
} // namespace nn
