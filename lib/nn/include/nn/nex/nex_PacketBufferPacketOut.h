#pragma once

#include "decomp.h"
#include "nn/nex/nex_PacketOut.h"

namespace nn {
namespace nex {
// A packet in the memory of the PacketBufferManager.
// (the destructor is a nop that falls into ~PacketOut)
// RTTI N2nn3nex21PacketBufferPacketOutE @ 0x008CEB38
// vtable 0x008FDEEC (vptr 0x008FDEF4), offset_to_top 0, 2 entries
class PacketBufferPacketOut : public ::nn::nex::PacketOut
{
public:
    // (inline)
    PacketBufferPacketOut() {}
    // the timeout comes from the PacketBufferManager too
    PacketBufferPacketOut(nn::nex::PRUDPEndPoint* pEndPoint, nn::nex::PacketType type, unsigned short flags, nn::nex::Buffer* pPayload, unsigned int value); // 0x0039A6E4 (name is ours)
    virtual ~PacketBufferPacketOut(); // 0x003D7880 slot 0x00
    // 0x0039A7C0 slot 0x04 (deleting dtor)
};
ASSERT_SIZE(PacketBufferPacketOut, 0x150);
} // namespace nex
} // namespace nn
