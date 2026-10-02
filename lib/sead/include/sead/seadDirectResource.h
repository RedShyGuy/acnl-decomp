#pragma once

#include "decomp.h"
#include "sead/seadResource.h"

namespace sead {
// RTTI N4sead14DirectResourceE @ 0x008D16EC
// vtable 0x009057A4 (vptr 0x009057AC), offset_to_top 0, 6 entries
class DirectResource : public ::sead::Resource
{
public:
    DirectResource(); // ctor candidate(s) 0x005448A8 (unverified)
    virtual void vf_0x00(); // 0x0074BE14 slot 0x00 | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x04(); // 0x0074BDC0 slot 0x04 | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x08(); // 0x005448FC slot 0x08 | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x0C(); // 0x005448CC slot 0x0C | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x10(); // 0x0074BE0C slot 0x10 | virtual slot, introduced by sead::DirectResource
    virtual void vf_0x14(); // 0x00544890 slot 0x14 | virtual slot, introduced by sead::DirectResource
};
} // namespace sead
