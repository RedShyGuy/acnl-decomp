#include "nn/nex/nex_SecurityContext.h"

namespace nn {
namespace nex {
// 0x0037AFD0
// 0x0037AFB8 (deleting dtor)
nn::nex::SecurityContext::~SecurityContext()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
