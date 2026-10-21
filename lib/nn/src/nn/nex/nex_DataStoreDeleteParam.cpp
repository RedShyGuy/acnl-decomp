#include "nn/nex/nex_DataStoreDeleteParam.h"

namespace nn {
namespace nex {
// 0x003962B0 slot 0x00
// 0x003962AC (deleting dtor)
nn::nex::DataStoreDeleteParam::~DataStoreDeleteParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x003B85F8
template <>
DECOMP_NOINLINE void DDLClassMetaType<DataStoreDeleteParam>::Add(Message* pMessage, const nn::nex::DataStoreDeleteParam& value)
{
    DataStoreDeleteParam::Add(pMessage, value);
}

} // namespace nex
} // namespace nn

namespace std {
// 0x008314C4
template <>
DECOMP_NOINLINE void vector<nn::nex::DataStoreDeleteParam, nn::nex::MemAllocator<nn::nex::DataStoreDeleteParam> >::_C_insert_aux(nn::nex::DataStoreDeleteParam* pos, const nn::nex::DataStoreDeleteParam& value)
{
    InsertAuxInline(pos, value);
}
} // namespace std
