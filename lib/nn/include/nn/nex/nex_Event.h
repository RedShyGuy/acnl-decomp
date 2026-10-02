#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex5EventE @ 0x008CF52C
// vtable 0x008FF9C8 (vptr 0x008FF9D0), offset_to_top 0, 2 entries
class Event : public ::nn::nex::RootObject
{
public:
    Event(); // ctor candidate(s) 0x0035C774 (unverified)
    virtual void vf_0x00(); // 0x003CE9CC slot 0x00 | virtual slot, introduced by nn::nex::Event
    virtual void vf_0x04(); // 0x003CE9B4 slot 0x04 | virtual slot, introduced by nn::nex::Event
    Event(nn::nex::EventHandler*, unsigned, unsigned); // 0x006F899C | mk7dlp:bytes [tier B]
};
} // namespace nex
} // namespace nn
