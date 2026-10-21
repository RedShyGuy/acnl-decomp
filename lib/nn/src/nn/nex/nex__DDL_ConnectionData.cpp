#include "nn/nex/nex__DDL_ConnectionData.h"
#include "nn/nex/nex_ConnectionData.h"

namespace nn {
namespace nex {
// 0x00394CFC slot 0x00
// 0x00394CDC (deleting dtor)
nn::nex::_DDL_ConnectionData::~_DDL_ConnectionData()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
