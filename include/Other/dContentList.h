#pragma once

#include "decomp.h"
#include "Other/dInOutWindowInButton.h"

// RTTI 11ContentList @ 0x008CB2B4
// vtable 0x008ECAF4 (vptr 0x008ECAFC), offset_to_top 0, 17 entries
class ContentList : public ::InOutWindowInButton<4>
{
public:
    ContentList(); // ctor address unknown
    virtual ~ContentList(); // 0x001C86EC slot 0x00 | slot vf_0x00 of InOutWindow
    // 0x001C8670 slot 0x04 | slot vf_0x04 of InOutWindow (deleting dtor)
    virtual void vf_0x18(); // 0x001C85F4 slot 0x18 | virtual slot, introduced by InOutWindow
    virtual void vf_0x28(); // 0x001C7F84 slot 0x28 | virtual slot, introduced by InOutWindow
    virtual void vf_0x34(); // 0x0070E6B4 slot 0x34 | virtual slot, introduced by InOutWindowInButton<4>
    virtual void vf_0x38(); // 0x0070E6A4 slot 0x38 | virtual slot, introduced by InOutWindowInButton<4>
    virtual void vf_0x3C(); // 0x0070E69C slot 0x3C | virtual slot, introduced by InOutWindowInButton<4>
    virtual void vf_0x40(); // 0x001C81B8 slot 0x40 | virtual slot, introduced by InOutWindowInButton<4>
};
