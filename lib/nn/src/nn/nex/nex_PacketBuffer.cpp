#include "nn/nex/nex_PacketBuffer.h"

namespace nn {
namespace nex {
// 0x0035D64C | fefates:bytes [tier B]
nn::nex::PacketBuffer::PacketBuffer(unsigned int size) : Buffer(size, FLAG_USE_POOL)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003D0074
// 0x0035D6B0 (deleting dtor)
nn::nex::PacketBuffer::~PacketBuffer()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
