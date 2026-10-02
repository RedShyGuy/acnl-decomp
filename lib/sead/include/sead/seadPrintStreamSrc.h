#pragma once

#include "decomp.h"
#include "sead/seadStreamSrc.h"

namespace sead {
// RTTI N4sead14PrintStreamSrcE @ 0x008D1744
// vtable 0x009058EC (vptr 0x009058F4), offset_to_top 0, 6 entries
class PrintStreamSrc : public ::sead::StreamSrc
{
public:
    PrintStreamSrc(); // ctor candidate(s) 0x0079A3E4 (unverified)
    virtual void vf_0x00(); // 0x00545B44 slot 0x00 | virtual slot, introduced by sead::PrintStreamSrc
    virtual void vf_0x04(); // 0x00545B5C slot 0x04 | virtual slot, introduced by sead::PrintStreamSrc
    virtual void vf_0x08(); // 0x00545B4C slot 0x08 | virtual slot, introduced by sead::PrintStreamSrc
    virtual void vf_0x0C(); // 0x00545B64 slot 0x0C | virtual slot, introduced by sead::PrintStreamSrc
    virtual void vf_0x10(); // 0x00545B54 slot 0x10 | virtual slot, introduced by sead::PrintStreamSrc
    virtual void vf_0x14(); // 0x00562A54 slot 0x14 | virtual slot, introduced by sead::PrintStreamSrc
};
} // namespace sead
