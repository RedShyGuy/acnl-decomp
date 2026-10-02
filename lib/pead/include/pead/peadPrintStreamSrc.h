#pragma once

#include "decomp.h"
#include "pead/peadStreamSrc.h"

namespace pead {
// RTTI N4pead14PrintStreamSrcE @ 0x008D1184
// vtable 0x00904A58 (vptr 0x00904A60), offset_to_top 0, 6 entries
class PrintStreamSrc : public ::pead::StreamSrc
{
public:
    PrintStreamSrc(); // ctor candidate(s) 0x0079A3FC (unverified)
    virtual void vf_0x00(); // 0x005387D4 slot 0x00 | virtual slot, introduced by pead::PrintStreamSrc
    virtual void vf_0x04(); // 0x005387EC slot 0x04 | virtual slot, introduced by pead::PrintStreamSrc
    virtual void vf_0x08(); // 0x005387DC slot 0x08 | virtual slot, introduced by pead::PrintStreamSrc
    virtual void vf_0x0C(); // 0x005387F4 slot 0x0C | virtual slot, introduced by pead::PrintStreamSrc
    virtual void vf_0x10(); // 0x005387E4 slot 0x10 | virtual slot, introduced by pead::PrintStreamSrc
    virtual void vf_0x14(); // 0x0053DC24 slot 0x14 | virtual slot, introduced by pead::PrintStreamSrc
};
} // namespace pead
