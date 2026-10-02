#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectAdapter.h"
#include "nn/nex/nex_Data.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14AnyDataAdapterE @ 0x008CE2CC
// vtable 0x008FCA3C (vptr 0x008FCA44), offset_to_top 0, 4 entries
class AnyDataAdapter : public ::nn::nex::AnyObjectAdapter<nn::nex::Data, nn::nex::String>
{
public:
    AnyDataAdapter(); // ctor address unknown
    virtual ~AnyDataAdapter(); // 0x00370800 slot 0x00 | fefates:bytes
    virtual void vf_0x04(); // 0x003707C8 slot 0x04 | virtual slot, introduced by nn::nex::AnyDataAdapter
    virtual void StreamIn(nn::nex::Message*, const nn::nex::Data&) const; // 0x0072ABF0 slot 0x08 | fefates:bytes
    virtual void StreamOut(nn::nex::Message*, nn::nex::Data**) const; // 0x0072AC9C slot 0x0C | fefates:bytes
    void UnregisterAllPrototypes(); // 0x003705E8 | fefates:bytes [tier B]
    void FindPrototype(const nn::nex::String&) const; // 0x0072AB70 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
