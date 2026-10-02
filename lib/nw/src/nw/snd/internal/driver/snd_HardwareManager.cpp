#include "nn/snd/CTR/snd_DspFxReverb.h"
#include "nn/snd/CTR/snd_DspFxDelay.h"
#include "nw/snd/internal/driver/snd_HardwareManager.h"

namespace nw {
namespace snd {
namespace internal {
namespace driver {
// 0x004CCE78 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::HardwareManager::ClearEffect(nw::snd::AuxBus, int)
{
}

// 0x004CCEEC | fefates:bytes [tier B]
void nw::snd::internal::driver::HardwareManager::AppendEffect(nw::snd::AuxBus, nn::snd::CTR::DspFxDelay*, const nn::snd::CTR::DspFxDelay::Param&)
{
}

// 0x004CCFBC | fefates:bytes [tier B]
void nw::snd::internal::driver::HardwareManager::AppendEffect(nw::snd::AuxBus, nn::snd::CTR::DspFxReverb*, const nn::snd::CTR::DspFxReverb::Param&)
{
}

// 0x004CD154 | nintendogs:callseq [tier A]
void nw::snd::internal::driver::HardwareManager::AppendEffect(nw::snd::AuxBus, nw::snd::FxBase*)
{
}

// 0x004CD204 | nintendogs:bytes-fuzzy [tier B]
void nw::snd::internal::driver::HardwareManager::SetOutputMode(nw::snd::OutputMode)
{
}

// 0x004CD2D0 | nintendogs:callseq-callee [tier A]
void nw::snd::internal::driver::HardwareManager::FinalizeEffect(nw::snd::AuxBus)
{
}

// 0x004CD3C0 | fefates:bytes [tier B]
void nw::snd::internal::driver::HardwareManager::AuxCallbackFunc(nn::snd::CTR::AuxBusData*, int, unsigned int)
{
}

// 0x004CD480 | nintendogs:bytes-fuzzy [tier B]
void nw::snd::internal::driver::HardwareManager::SetMasterVolume(float, int)
{
}

// 0x004CD50C | fefates:bytes [tier B]
void nw::snd::internal::driver::HardwareManager::Update()
{
}

// 0x004CD710 | nintendogs:bytes-fuzzy [tier A]
void nw::snd::internal::driver::HardwareManager::Finalize()
{
}

// 0x004CD768 | fefates:bytes [tier B]
nw::snd::internal::driver::HardwareManager::HardwareManager()
{
}

// 0x00741458 | fefates:bytes [tier B]
void nw::snd::internal::driver::HardwareManager::GetOutputVolume() const
{
}

} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
