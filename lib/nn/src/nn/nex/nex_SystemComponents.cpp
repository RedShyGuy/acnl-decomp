#include "nn/nex/nex_SystemComponents.h"

namespace nn {
namespace nex {
// 0x0039828C
// 0x00383A28 (deleting dtor)
nn::nex::SystemComponents::~SystemComponents()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
