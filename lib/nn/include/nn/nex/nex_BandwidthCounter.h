#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16BandwidthCounterE @ 0x008CE554
// vtable 0x008FD084 (vptr 0x008FD08C), offset_to_top 0, 2 entries
class BandwidthCounter : public ::nn::nex::RootObject
{
public:
    BandwidthCounter(); // ctor address unknown
    virtual void vf_0x00(); // 0x0037EF90 slot 0x00 | fefates:callgraph
    virtual ~BandwidthCounter(); // 0x0037EF60 slot 0x04 | slot vf_0x04 of nn::nex::BandwidthCounter
    void operator +=(unsigned); // 0x0037F05C | mk7dlp:bytes [tier A]
};
} // namespace nex
} // namespace nn
