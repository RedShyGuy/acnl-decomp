#pragma once

#include "decomp.h"
#include "nn/nex/nex_Data.h"

namespace nn {
namespace nex {
// RTTI N2nn3nex23_DDL_AuthenticationInfoE @ 0x008CED18
// vtable 0x008FE644 (vptr 0x008FE64C), offset_to_top 0, 8 entries
class _DDL_AuthenticationInfo : public ::nn::nex::Data
{
public:
    _DDL_AuthenticationInfo(); // ctor address unknown
    virtual void vf_0x00(); // 0x003B18A4 slot 0x00 | virtual slot, introduced by nn::nex::Data
    virtual ~_DDL_AuthenticationInfo(); // 0x003B1880 slot 0x04 | slot vf_0x04 of nn::nex::Data
    virtual void Clone() const; // 0x0072D31C slot 0x08 | fefates:bytes
    virtual void GetDataType() const; // 0x0072D2B4 slot 0x0C | mk7dlp:bytes
    virtual void vf_0x10(); // 0x0072D2E4 slot 0x10 | virtual slot, introduced by nn::nex::Data
    virtual void vf_0x14(); // 0x0072D4D0 slot 0x14 | virtual slot, introduced by nn::nex::Data
    virtual void StreamIn(nn::nex::Message*) const; // 0x0072D3D0 slot 0x18 | fefates:bytes
    virtual void vf_0x1C(); // 0x003B1664 slot 0x1C | virtual slot, introduced by nn::nex::Data
    void Extract(nn::nex::Message*, nn::nex::_DDL_AuthenticationInfo*); // 0x003B1674 | fefates:bytes [tier B]
};
} // namespace nex
} // namespace nn
