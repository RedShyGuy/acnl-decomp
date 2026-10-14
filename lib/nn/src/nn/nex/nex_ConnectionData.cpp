#include "nn/nex/nex_ConnectionData.h"

namespace nn {
namespace nex {
// 0x00371020 slot 0x00
// 0x00371000 (deleting dtor)
nn::nex::ConnectionData::~ConnectionData()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
