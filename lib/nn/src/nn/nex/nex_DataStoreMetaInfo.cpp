#include "nn/nex/nex_DataStoreMetaInfo.h"

namespace nn {
namespace nex {
// 0x00386B38
nn::nex::DataStoreMetaInfo::DataStoreMetaInfo(const nn::nex::DataStoreMetaInfo& other) : _DDL_DataStoreMetaInfo(other)
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x00386D44 slot 0x00
// 0x00386D34 (deleting dtor)
nn::nex::DataStoreMetaInfo::~DataStoreMetaInfo()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
