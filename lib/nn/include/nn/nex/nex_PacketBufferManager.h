#pragma once

#include "decomp.h"
#include "nn/nex/nex_PseudoSingleton.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19PacketBufferManagerE @ 0x008CE7E4
// vtable 0x008FD6C8 (vptr 0x008FD6D0), offset_to_top 0, 2 entries
class PacketBufferManager : public ::nn::nex::PseudoSingleton
{
public:
    PacketBufferManager(); // ctor address unknown
    virtual ~PacketBufferManager(); // 0x00394188 slot 0x00 | slot vf_0x00 of nn::nex::InstanceControl
    // 0x00394140 slot 0x04 | slot vf_0x04 of nn::nex::InstanceControl (deleting dtor)
};
} // namespace nex
} // namespace nn
