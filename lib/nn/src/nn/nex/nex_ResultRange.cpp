#include "nn/nex/nex_ResultRange.h"

namespace nn {
namespace nex {
// 0x0035BA7C slot 0x00
// 0x0035BA78 (deleting dtor)
nn::nex::ResultRange::~ResultRange()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn

namespace std {
// 0x0082F2BC (name is ours)
template <>
DECOMP_NOINLINE void list<nn::nex::ResultRange, nn::nex::MemAllocator<nn::nex::ResultRange> >::clear()
{
    ClearInline();
}
} // namespace std
