#pragma once

#include "decomp.h"
#include "nn/nex/nex_RefCountedObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex7NetworkE @ 0x008CF680
// vtable 0x008FFB68 (vptr 0x008FFB70), offset_to_top 0, 2 entries
class Network : public ::nn::nex::RefCountedObject
{
public:
    Network(); // ctor address unknown
    virtual ~Network(); // 0x003D3434 slot 0x00 | slot vf_0x00 of nn::nex::RefCountedObject
    // 0x003D3404 slot 0x04 | slot vf_0x04 of nn::nex::RefCountedObject (deleting dtor)
    void ReleaseInstance(); // 0x003D2980 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
