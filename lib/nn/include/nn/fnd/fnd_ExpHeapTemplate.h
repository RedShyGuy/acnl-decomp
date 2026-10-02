#pragma once

#include "decomp.h"

namespace nn {
namespace fnd {
// Instantiations found in the binary:
//   nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::NoLock>  typeinfo 0x008CDE58  vtable 0x008FC078
//   nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >  typeinfo 0x008CDE84  vtable 0x008FC0B4
//   nn::fnd::ExpHeapTemplate<nn::os::LockPolicy::Object<nn::os::CriticalSection> >::Allocator  typeinfo 0x008CDE78  vtable 0x008FC09C
template <typename T0>
class ExpHeapTemplate
{
public:
    // TODO: members unknown
};
} // namespace fnd
} // namespace nn
