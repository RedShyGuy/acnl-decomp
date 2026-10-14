#include "nn/nex/nex_DataStoreReqUpdateInfo.h"

namespace nn {
namespace nex {
// 0x0039C238 slot 0x00
// 0x0039C1F8 (deleting dtor)
nn::nex::DataStoreReqUpdateInfo::~DataStoreReqUpdateInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
