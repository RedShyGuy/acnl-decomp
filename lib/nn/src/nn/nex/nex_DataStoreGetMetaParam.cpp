#include "nn/nex/nex_DataStoreGetMetaParam.h"

namespace nn {
namespace nex {
// 0x003983E8 slot 0x00
// 0x003983D8 (deleting dtor)
nn::nex::DataStoreGetMetaParam::~DataStoreGetMetaParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003BBA30
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreGetMetaParam>::Add(Message* pMessage, const nn::nex::DataStoreGetMetaParam& value)
{
    DataStoreGetMetaParam::Add(pMessage, value);
}

} // namespace nex
} // namespace nn
