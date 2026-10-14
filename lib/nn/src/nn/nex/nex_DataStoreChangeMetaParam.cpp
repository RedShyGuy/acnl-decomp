#include "nn/nex/nex_DataStoreChangeMetaParam.h"

namespace nn {
namespace nex {
// 0x003B22E0 slot 0x00
// 0x003B22D0 (deleting dtor)
nn::nex::DataStoreChangeMetaParam::~DataStoreChangeMetaParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
