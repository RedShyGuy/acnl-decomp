#include "nn/nex/nex_Event.h"

namespace nn {
namespace nex {
// 0x003CE9CC
// 0x003CE9B4 (deleting dtor)
nn::nex::Event::~Event()
{
    // nothing to do: the members and bases are destroyed / constructed by the compiler
}

} // namespace nex
} // namespace nn
