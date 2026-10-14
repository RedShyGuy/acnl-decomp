#include "nn/nex/nex_DataStorePasswordInfo.h"

namespace nn {
namespace nex {
// 0x00398424 slot 0x00
// 0x00398420 (deleting dtor)
nn::nex::DataStorePasswordInfo::~DataStorePasswordInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
