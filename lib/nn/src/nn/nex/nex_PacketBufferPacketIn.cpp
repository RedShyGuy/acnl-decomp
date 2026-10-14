#include "nn/nex/nex_PacketBufferPacketIn.h"

namespace nn {
namespace nex {
// 0x0039797C slot 0x00
// 0x003978DC (deleting dtor)
nn::nex::PacketBufferPacketIn::~PacketBufferPacketIn()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
