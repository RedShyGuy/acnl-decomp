#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex18NATTraversalEngineE @ 0x008CE724
// vtable 0x008FD51C (vptr 0x008FD524), offset_to_top 0, 8 entries
class NATTraversalEngine : public ::nn::nex::RootObject
{
public:
    virtual ~NATTraversalEngine(); // 0x0039006C slot 0x00 | fefates:bytes
    // 0x0039003C slot 0x04 | slot vf_0x04 of nn::nex::NATTraversalEngine (deleting dtor)
    virtual void vf_0x08(); // 0x0038D5B0 slot 0x08 | virtual slot, introduced by nn::nex::NATTraversalEngine
    virtual void vf_0x0C(); // 0x0038E7F4 slot 0x0C | virtual slot, introduced by nn::nex::NATTraversalEngine
    virtual void vf_0x10(); // 0x0038D484 slot 0x10 | virtual slot, introduced by nn::nex::NATTraversalEngine
    virtual void vf_0x14(); // 0x0038E1B0 slot 0x14 | virtual slot, introduced by nn::nex::NATTraversalEngine
    virtual void vf_0x18(); // 0x0038FB78 slot 0x18 | virtual slot, introduced by nn::nex::NATTraversalEngine
    virtual void vf_0x1C(); // 0x0038FBC0 slot 0x1C | virtual slot, introduced by nn::nex::NATTraversalEngine
    void StartSendProbe(const nn::nex::StationURL&); // 0x0038DE30 | fefates:bytes-fuzzy [tier B]
    NATTraversalEngine(); // 0x0038FE80 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
