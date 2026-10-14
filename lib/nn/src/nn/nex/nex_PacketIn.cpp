#include "nn/nex/nex_PacketIn.h"

namespace nn {
namespace nex {
// 0x003D57A0 slot 0x00
// 0x003D56FC (deleting dtor)
nn::nex::PacketIn::~PacketIn()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
