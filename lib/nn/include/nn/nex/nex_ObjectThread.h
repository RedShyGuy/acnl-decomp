#pragma once

#include "decomp.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::ObjectThread<nn::nex::JobNameResolve, int>  typeinfo 0x008CE120  vtable 0x008FC5AC
//   nn::nex::ObjectThread<nn::nex::TransportBufferMultiThread, int>  typeinfo 0x008CE138  vtable 0x008FC5D4
//   nn::nex::ObjectThread<nn::nex::TransportBufferThread, void*>  typeinfo 0x008CE12C  vtable 0x008FC5C0
//   nn::nex::ObjectThread<nn::nex::WorkerThreads, int>  typeinfo 0x008CE114  vtable 0x008FC598
template <typename T0, typename T1>
class ObjectThread
{
public:
    // TODO: members unknown
};
} // namespace nex
} // namespace nn
