#include "nn/nex/nex_MutexPrimitive.h"

namespace nn {
namespace nex {
// 0x003718FC
// 0x003718D8 (deleting dtor)
nn::nex::MutexPrimitive::~MutexPrimitive()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
