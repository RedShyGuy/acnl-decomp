#include "nn/nex/nex_LockChecker.h"

namespace nn {
namespace nex {
// 0x0035874C
// 0x00358734 (deleting dtor)
nn::nex::LockChecker::~LockChecker()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
