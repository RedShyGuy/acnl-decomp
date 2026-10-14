#include "nn/nex/nex_RVConnectionData.h"

namespace nn {
namespace nex {
// 0x003839B8 slot 0x00
// 0x003839A8 (deleting dtor)
nn::nex::RVConnectionData::~RVConnectionData()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
