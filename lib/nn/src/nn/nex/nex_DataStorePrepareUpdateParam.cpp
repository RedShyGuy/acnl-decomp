#include "nn/nex/nex_DataStorePrepareUpdateParam.h"

namespace nn {
namespace nex {
// 0x003BCF78 slot 0x00
// 0x003BCF28 (deleting dtor)
nn::nex::DataStorePrepareUpdateParam::~DataStorePrepareUpdateParam()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
