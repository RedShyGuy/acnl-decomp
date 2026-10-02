#include "nw/snd/snd_FxBase.h"
#include "nw/snd/snd_FxDelay.h"

namespace nw {
namespace snd {
// 0x004C3ECC slot 0x00 | virtual slot, introduced by nw::snd::FxDelay
void nw::snd::FxDelay::vf_0x00()
{
}

// 0x004C3E5C slot 0x04 | nintendogs:bytes
nw::snd::FxDelay::~FxDelay()
{
}

// 0x004C3A38 slot 0x08 | nintendogs:bytes
void nw::snd::FxDelay::Initialize()
{
}

// 0x004C3C38 slot 0x0C | nintendogs:bytes
void nw::snd::FxDelay::Finalize()
{
}

// 0x004C3AE0 slot 0x10 | nintendogs:bytes
void nw::snd::FxDelay::UpdateBuffer(int, nn::snd::CTR::AuxBusData*, int, nw::snd::SampleFormat, float, nw::snd::OutputMode)
{
}

// 0x004C3A34 slot 0x14 | slot vf_0x14 of nw::snd::FxDelay
void nw::snd::FxDelay::OnChangeOutputMode()
{
}

// 0x004C3BE8 | nintendogs:bytes [tier B]
void nw::snd::FxDelay::AssignWorkBuffer(unsigned, unsigned)
{
}

// 0x004C3C1C | nintendogs:bytes [tier B]
void nw::snd::FxDelay::GetRequiredMemSize()
{
}

// 0x004C3C8C | nintendogs:bytes [tier B]
void nw::snd::FxDelay::SetParam(const nw::snd::FxDelay::Param&)
{
}

// 0x004C3DB4 | nintendogs:bytes [tier B]
nw::snd::FxDelay::FxDelay()
{
}

} // namespace snd
} // namespace nw
