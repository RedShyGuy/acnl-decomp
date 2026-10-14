#include "nn/nex/nex_PacketBufferPacketOut.h"

namespace nn {
namespace nex {
// 0x003D7880 slot 0x00
// 0x0039A7C0 (deleting dtor)
nn::nex::PacketBufferPacketOut::~PacketBufferPacketOut()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
