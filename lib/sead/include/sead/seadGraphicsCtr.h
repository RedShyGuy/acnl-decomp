#pragma once

#include "decomp.h"
#include "sead/seadGraphics.h"

namespace sead {
// RTTI N4sead11GraphicsCtrE @ 0x008D1404
// vtable 0x00904FAC (vptr 0x00904FB4), offset_to_top 0, 28 entries
class GraphicsCtr : public ::sead::Graphics
{
public:
    GraphicsCtr(); // ctor candidate(s) 0x0011E8A0 (unverified)
    virtual ~GraphicsCtr(); // 0x0054045C slot 0x00 | slot vf_0x00 of sead::IDisposer
    // 0x00540440 slot 0x04 | slot vf_0x04 of sead::IDisposer (deleting dtor)
    virtual void vf_0x0C(); // 0x0053FE1C slot 0x0C | virtual slot, introduced by sead::Graphics
    virtual void vf_0x10(); // 0x0053FEAC slot 0x10 | virtual slot, introduced by sead::Graphics
    virtual void setScissorImpl(float, float, float, float); // 0x0053FE20 slot 0x14 | mk7dlp:bytes-fuzzy
    virtual void vf_0x18(); // 0x0054026C slot 0x18 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x1C(); // 0x00540308 slot 0x1C | virtual slot, introduced by sead::Graphics
    virtual void vf_0x20(); // 0x0053FF7C slot 0x20 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x24(); // 0x00540434 slot 0x24 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x28(); // 0x0054006C slot 0x28 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x2C(); // 0x0053FFCC slot 0x2C | virtual slot, introduced by sead::Graphics
    virtual void vf_0x30(); // 0x00540220 slot 0x30 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x34(); // 0x00540010 slot 0x34 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x38(); // 0x00540224 slot 0x38 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x3C(); // 0x00540180 slot 0x3C | virtual slot, introduced by sead::Graphics
    virtual void vf_0x40(); // 0x00540304 slot 0x40 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x44(); // 0x005403A0 slot 0x44 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x48(); // 0x0053FFC0 slot 0x48 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x4C(); // 0x0053FEFC slot 0x4C | virtual slot, introduced by sead::Graphics
    virtual void vf_0x50(); // 0x005400DC slot 0x50 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x54(); // 0x00540228 slot 0x54 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x58(); // 0x005400E0 slot 0x58 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x5C(); // 0x0054034C slot 0x5C | virtual slot, introduced by sead::Graphics
    virtual void vf_0x60(); // 0x005402B0 slot 0x60 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x64(); // 0x005401CC slot 0x64 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x68(); // 0x005400D8 slot 0x68 | virtual slot, introduced by sead::Graphics
    virtual void vf_0x6C(); // 0x0054043C slot 0x6C | virtual slot, introduced by sead::Graphics
};
} // namespace sead
