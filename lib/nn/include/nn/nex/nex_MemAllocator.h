#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// The allocator of the RW STL containers of nex (MemoryManager memory; the containers use
// MemoryManager directly). Instantiations found in the binary:
//   nn::nex::MemAllocator<nn::nex::URLProbe>  typeinfo 0x008CE100
template <typename T>
class MemAllocator
{
};
} // namespace nex
} // namespace nn
