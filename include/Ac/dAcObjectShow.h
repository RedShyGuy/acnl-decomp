#pragma once

#include "decomp.h"
#include "Other/dActor.h"
#include "Resource/dResourceGetSklMat.h"

// RTTI 12AcObjectShow @ 0x008CB3E0
// vtable 0x008ED2C4 (vptr 0x008ED2CC), offset_to_top 0, 19 entries
// vtable 0x008ED31C (vptr 0x008ED324), offset_to_top -72, 9 entries
// vtable 0x008ED370 (vptr 0x008ED378), offset_to_top -852, 11 entries
class AcObjectShow : public ::Actor, public ::ResourceGetSklMat
{
public:
    AcObjectShow(); // ctor address unknown
    virtual ~AcObjectShow(); // 0x001F7A8C slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001F7A78 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x001F778C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x001F790C slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001F7820 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001F7638 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x44(); // 0x001F6494 slot 0x44 | virtual slot, introduced by AcObjectShow
    virtual void vf_0x48(); // 0x002F71F4 slot 0x48 | virtual slot, introduced by AcObjectShow
};
