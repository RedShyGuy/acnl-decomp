#include "nn/nex/nex_DataStoreSpecificMetaInfoV1.h"

namespace nn {
namespace nex {
// 0x003BCFE8 slot 0x00
// 0x003BCFE4 (deleting dtor)
nn::nex::DataStoreSpecificMetaInfoV1::~DataStoreSpecificMetaInfoV1()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
