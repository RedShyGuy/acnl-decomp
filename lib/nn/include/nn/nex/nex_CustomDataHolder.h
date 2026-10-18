#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectHolder.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// An AnyObjectHolder<Data, String> for custom data of a type T (the login sends an
// AuthenticationInfo). Only the vtable is its own.
// Instantiations found in the binary:
//   nn::nex::CustomDataHolder<nn::nex::AuthenticationInfo>  typeinfo 0x008CE56C  vtable 0x008FD0A4
//   nn::nex::CustomDataHolder<nn::nex::UserMessage>  typeinfo 0x008CE560  vtable 0x008FD094
template <typename T0>
class CustomDataHolder : public ::nn::nex::AnyObjectHolder<Data, String>
{
public:
    // (inline)
    CustomDataHolder() {}
    virtual ~CustomDataHolder() {}
};
} // namespace nex
} // namespace nn
