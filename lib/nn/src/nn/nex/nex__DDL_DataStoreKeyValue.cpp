#include "nn/nex/nex__DDL_DataStoreKeyValue.h"
#include "nn/nex/nex_DataStoreKeyValue.h"

namespace nn {
namespace nex {
// 0x0039ED80 slot 0x00
// 0x0039ED58 (deleting dtor)
nn::nex::_DDL_DataStoreKeyValue::~_DDL_DataStoreKeyValue()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
