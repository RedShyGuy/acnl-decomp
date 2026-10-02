#include "nn/snd/CTR/snd_MasterManager.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00461A48 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::Initialize()
{
}

// 0x00461B28 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::ClearEffect(nn::snd::CTR::AuxBusId)
{
}

// 0x00461B88 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::GetDspCycles()
{
}

// 0x00461C08 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::ExecuteEffect(nn::snd::CTR::AuxBusId, unsigned)
{
}

// 0x00461C7C | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::GetAuxCallback(nn::snd::CTR::AuxBusId, void(**)(nn::snd::CTR::AuxBusData*, int, unsigned), unsigned*)
{
}

// 0x00461C94 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::AuxUserCallback(nn::snd::CTR::AuxBusId, unsigned)
{
}

// 0x00461CB0 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::SetMasterVolume(float)
{
}

// 0x00461CD0 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::ClearAuxCallback(nn::snd::CTR::AuxBusId)
{
}

// 0x00461D40 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::RegisterAuxCallback(nn::snd::CTR::AuxBusId, void(*)(nn::snd::CTR::AuxBusData*, int, unsigned), unsigned)
{
}

// 0x00461D64 | nintendogs:callgraph [tier A]
void nn::snd::CTR::MasterManager::SetIsHeadsetConnected(bool)
{
}

// 0x00461D74 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::UpdateDroppedSoundFrameCount()
{
}

// 0x00461DA0 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::Finalize()
{
}

// 0x00461DD4 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::SetEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::FxDelay*)
{
}

// 0x00461E78 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManager::SetEffect(nn::snd::CTR::AuxBusId, nn::snd::CTR::FxReverb*)
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
