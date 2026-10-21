#include "nn/nex/nex_LocalClock.h"

namespace nn {
namespace nex {
// 0x003550F8
// 0x003550D4 (deleting dtor)
nn::nex::LocalClock::~LocalClock()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
