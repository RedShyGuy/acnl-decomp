#pragma once

#include "decomp.h"
#include "nn/nex/nex_NonCopyable.h"
#include "nn/nex/nex_RootObject.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex14HttpConnectionE @ 0x008CE2FC
// vtable 0x008FCB10 (vptr 0x008FCB18), offset_to_top 0, 23 entries
class HttpConnection : public ::nn::nex::RootObject, public ::nn::nex::NonCopyable
{
public:
    struct Method { u32 _unknown; }; // TODO: real type unknown (placeholder)
    HttpConnection(); // ctor address unknown
    virtual ~HttpConnection(); // 0x00371058 slot 0x00 | slot vf_0x00 of nn::nex::HttpConnection
    // 0x00371040 slot 0x04 | slot vf_0x04 of nn::nex::HttpConnection (deleting dtor)
    virtual void vf_0x08(); // 0x0011C12F slot 0x08 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x0C(); // 0x0011C12F slot 0x0C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0072AFA0 slot 0x18 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x24(); // 0x0011C12F slot 0x24 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x28(); // 0x0011C12F slot 0x28 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x2C(); // 0x0011C12F slot 0x2C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x30(); // 0x0011C12F slot 0x30 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x34(); // 0x0011C12F slot 0x34 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x38(); // 0x0011C12F slot 0x38 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x3C(); // 0x0011C12F slot 0x3C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x40(); // 0x0011C12F slot 0x40 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x44(); // 0x0011C12F slot 0x44 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x48(); // 0x0011C12F slot 0x48 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x4C(); // 0x0011C12F slot 0x4C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x50(); // 0x0037103C slot 0x50 | virtual slot, introduced by nn::nex::HttpConnection
    virtual void AddRootCa(const unsigned char*, int); // 0x0011C12F slot 0x54 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x58(); // 0x0011C12F slot 0x58 | slot vf_0x00 of ChangeRentalBase
    void GetBoundaryString() const; // 0x0072AFA8 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
