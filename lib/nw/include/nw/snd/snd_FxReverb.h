#pragma once

#include "decomp.h"
#include "nw/snd/snd_FxBase.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd8FxReverbE @ 0x008D0970
// vtable 0x00902FE4 (vptr 0x00902FEC), offset_to_top 0, 6 entries
class FxReverb : public ::nw::snd::FxBase
{
public:
    struct Param { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual void vf_0x00(); // 0x004C4934 slot 0x00 | virtual slot, introduced by nw::snd::FxReverb
    virtual ~FxReverb(); // 0x004C48BC slot 0x04 | nintendogs:bytes
    virtual void Initialize(); // 0x004C3F3C slot 0x08 | nintendogs:bytes
    virtual void Finalize(); // 0x004C4570 slot 0x0C | nintendogs:bytes
    virtual void UpdateBuffer(int, nn::snd::CTR::AuxBusData*, int, nw::snd::SampleFormat, float, nw::snd::OutputMode); // 0x004C4054 slot 0x10 | slot vf_0x10 of nw::snd::FxReverb
    virtual void OnChangeOutputMode(); // 0x004C3A34 slot 0x14 | slot vf_0x14 of nw::snd::FxDelay
    void AssignWorkBuffer(unsigned, unsigned); // 0x004C449C | nintendogs:bytes [tier B]
    void GetRequiredMemSize(); // 0x004C44D0 | nintendogs:bytes [tier B]
    void SetParam(const nw::snd::FxReverb::Param&); // 0x004C45CC | nintendogs:bytes [tier B]
    FxReverb(); // 0x004C4758 | nintendogs:bytes [tier B]
};
} // namespace snd
} // namespace nw
