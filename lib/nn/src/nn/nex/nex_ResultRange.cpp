#include "nn/nex/nex_ResultRange.h"

namespace nn {
namespace nex {
// 0x0035BA7C slot 0x00
// 0x0035BA78 (deleting dtor)
nn::nex::ResultRange::~ResultRange()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

// 0x0082F2BC (name is ours)
template <>
DECOMP_NOINLINE void qList<ResultRange>::clear()
{
    ClearInline();
}

} // namespace nex
} // namespace nn
