#pragma once

#include "decomp.h"
#include "nw/snd/snd_FxBase.h"

namespace nw {
namespace snd {
// RTTI N2nw3snd7FxDelayE @ 0x008D0964
// vtable 0x00902FC4 (vptr 0x00902FCC), offset_to_top 0, 6 entries
class FxDelay : public ::nw::snd::FxBase
{
public:
    struct Param { u32 _unknown; }; // TODO: real type unknown (placeholder)
    virtual void vf_0x00(); // 0x004C3ECC slot 0x00 | virtual slot, introduced by nw::snd::FxDelay
    virtual ~FxDelay(); // 0x004C3E5C slot 0x04 | nintendogs:bytes
    virtual void Initialize(); // 0x004C3A38 slot 0x08 | nintendogs:bytes
    virtual void Finalize(); // 0x004C3C38 slot 0x0C | nintendogs:bytes
    virtual void UpdateBuffer(int, nn::snd::CTR::AuxBusData*, int, nw::snd::SampleFormat, float, nw::snd::OutputMode); // 0x004C3AE0 slot 0x10 | nintendogs:bytes
    virtual void OnChangeOutputMode(); // 0x004C3A34 slot 0x14 | slot vf_0x14 of nw::snd::FxDelay
    void AssignWorkBuffer(unsigned, unsigned); // 0x004C3BE8 | nintendogs:bytes [tier B]
    void GetRequiredMemSize(); // 0x004C3C1C | nintendogs:bytes [tier B]
    void SetParam(const nw::snd::FxDelay::Param&); // 0x004C3C8C | nintendogs:bytes [tier B]
    FxDelay(); // 0x004C3DB4 | nintendogs:bytes [tier B]
};
} // namespace snd
} // namespace nw
