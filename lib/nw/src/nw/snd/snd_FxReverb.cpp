#include "nw/snd/snd_FxBase.h"
#include "nw/snd/snd_FxReverb.h"

namespace nw {
namespace snd {
// 0x004C4934 slot 0x00 | virtual slot, introduced by nw::snd::FxReverb
void nw::snd::FxReverb::vf_0x00()
{
}

// 0x004C48BC slot 0x04 | nintendogs:bytes
nw::snd::FxReverb::~FxReverb()
{
}

// 0x004C3F3C slot 0x08 | nintendogs:bytes
void nw::snd::FxReverb::Initialize()
{
}

// 0x004C4570 slot 0x0C | nintendogs:bytes
void nw::snd::FxReverb::Finalize()
{
}

// 0x004C4054 slot 0x10 | slot vf_0x10 of nw::snd::FxReverb
void nw::snd::FxReverb::UpdateBuffer(int, nn::snd::CTR::AuxBusData*, int, nw::snd::SampleFormat, float, nw::snd::OutputMode)
{
}

// 0x004C3A34 slot 0x14 | slot vf_0x14 of nw::snd::FxDelay
void nw::snd::FxReverb::OnChangeOutputMode()
{
}

// 0x004C449C | nintendogs:bytes [tier B]
void nw::snd::FxReverb::AssignWorkBuffer(unsigned, unsigned)
{
}

// 0x004C44D0 | nintendogs:bytes [tier B]
void nw::snd::FxReverb::GetRequiredMemSize()
{
}

// 0x004C45CC | nintendogs:bytes [tier B]
void nw::snd::FxReverb::SetParam(const nw::snd::FxReverb::Param&)
{
}

// 0x004C4758 | nintendogs:bytes [tier B]
nw::snd::FxReverb::FxReverb()
{
}

} // namespace snd
} // namespace nw
