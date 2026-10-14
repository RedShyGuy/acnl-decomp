#include "nn/nex/nex_DataStoreSpecificMetaInfo.h"

namespace nn {
namespace nex {
// 0x003B6F68 slot 0x00
// 0x003B6F64 (deleting dtor)
nn::nex::DataStoreSpecificMetaInfo::~DataStoreSpecificMetaInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
