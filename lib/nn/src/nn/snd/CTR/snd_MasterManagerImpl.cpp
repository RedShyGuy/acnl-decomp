#include "nn/snd/CTR/snd_MasterManagerImpl.h"

namespace nn {
namespace snd {
namespace CTR {
// 0x00462E3C | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManagerImpl::Initialize()
{
}

// 0x00462E64 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManagerImpl::AuxUserCallback(nn::snd::CTR::AuxBusId, unsigned)
{
}

// 0x00462EE4 | nintendogs:callgraph [tier A]
void nn::snd::CTR::MasterManagerImpl::InitializeParam()
{
}

// 0x004630CC | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManagerImpl::SetMasterVolume(float)
{
}

// 0x004630F4 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManagerImpl::ForceUpdateParams()
{
}

// 0x00463268 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManagerImpl::RegisterAuxCallback(nn::snd::CTR::AuxBusId, void(*)(nn::snd::CTR::AuxBusData*, int, unsigned), unsigned)
{
}

// 0x004632D4 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManagerImpl::SetOutputBufferCount(int)
{
}

// 0x00463304 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManagerImpl::EnableFx(nn::snd::CTR::AuxBusId, bool)
{
}

// 0x00463368 | nintendogs:bytes [tier A]
void nn::snd::CTR::MasterManagerImpl::Finalize()
{
}

} // namespace CTR
} // namespace snd
} // namespace nn
