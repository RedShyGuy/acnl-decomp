#pragma once

#include "decomp.h"
#include "nn/nex/nex_AnyObjectAdapter.h"
#include "nn/nex/nex_Gathering.h"
#include "nn/nex/nex_String.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex19AnyGatheringAdapterE @ 0x008CE778
// vtable 0x008FD5D0 (vptr 0x008FD5D8), offset_to_top 0, 4 entries
class AnyGatheringAdapter : public ::nn::nex::AnyObjectAdapter<nn::nex::Gathering, nn::nex::String>
{
public:
    AnyGatheringAdapter(); // ctor candidate(s) 0x0079E960 (unverified)
    virtual ~AnyGatheringAdapter(); // 0x00391560 slot 0x00 | slot vf_0x00 of nn::nex::AnyGatheringAdapter
    virtual void vf_0x04(); // 0x0039155C slot 0x04 | virtual slot, introduced by nn::nex::AnyGatheringAdapter
    virtual void StreamIn(nn::nex::Message*, const nn::nex::Gathering&) const; // 0x0072C890 slot 0x08 | fefates:bytes
    virtual void StreamOut(nn::nex::Message*, nn::nex::Gathering**) const; // 0x0072C998 slot 0x0C | fefates:bytes
};
} // namespace nex
} // namespace nn
