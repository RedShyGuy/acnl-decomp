#pragma once

#include "decomp.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex16ObjectThreadRootE @ 0x008CE5E4
// vtable 0x008FD1F0 (vptr 0x008FD1F8), offset_to_top 0, 3 entries
class ObjectThreadRoot : public ::nn::nex::RootObject
{
public:
    ObjectThreadRoot(); // ctor address unknown
    virtual ~ObjectThreadRoot(); // 0x00382DC4 slot 0x00 | fefates:callseq
    // 0x00382D90 slot 0x04 | slot vf_0x04 of nn::nex::ObjectThreadRoot (deleting dtor)
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    void LaunchImpl(); // 0x00382718 | fefates:bytes [tier B]
    void Wait(unsigned int); // 0x00382AA8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
