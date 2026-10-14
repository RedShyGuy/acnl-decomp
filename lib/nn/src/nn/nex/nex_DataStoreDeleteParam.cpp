#include "nn/nex/nex_DataStoreDeleteParam.h"

namespace nn {
namespace nex {
// 0x003962B0 slot 0x00
// 0x003962AC (deleting dtor)
nn::nex::DataStoreDeleteParam::~DataStoreDeleteParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
