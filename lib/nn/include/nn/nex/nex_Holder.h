#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// Instantiations found in the binary:
//   nn::nex::Holder<nn::nex::Data>  typeinfo 0x008CF588
//   nn::nex::Holder<nn::nex::Gathering>  typeinfo 0x008CF594
//
// The base of AnyObjectHolder<T0, T1> (the RTTI: a public base at offset 4, behind the vptr of the
// derived class; its own base is RootObject). The RTTI does not tell whether the pointer at +4
// belongs to it; nothing in the binary uses it on its own, so it is kept empty here.
template <typename T0>
class Holder : public RootObject
{
};
} // namespace nex
} // namespace nn
