#pragma once

#include "decomp.h"
#include "nn/nex/nex_Stream.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex15HighLevelStreamE @ 0x008CE4A8
// vtable 0x008FCEA0 (vptr 0x008FCEA8), offset_to_top 0, 7 entries
class HighLevelStream : public ::nn::nex::Stream
{
public:
    HighLevelStream(); // ctor candidate(s) 0x00378308 (unverified)
    virtual ~HighLevelStream(); // 0x0037846C slot 0x00 | slot vf_0x00 of nn::nex::Stream
    // 0x003783E8 slot 0x04 | slot vf_0x04 of nn::nex::Stream (deleting dtor)
    virtual void ReceiveIncomingPacket(unsigned short, unsigned char, nn::nex::Packet*); // 0x00377D88 slot 0x08 | slot vf_0x08 of nn::nex::Stream
    virtual void DoWork(); // 0x003781D0 slot 0x0C | slot vf_0x0C of nn::nex::Stream
    virtual void vf_0x14(); // 0x00377D84 slot 0x14 | virtual slot, introduced by nn::nex::HighLevelStream
    virtual void vf_0x18(); // 0x00377EEC slot 0x18 | virtual slot, introduced by nn::nex::HighLevelStream
};
} // namespace nex
} // namespace nn
