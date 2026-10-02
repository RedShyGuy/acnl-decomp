#pragma once

#include "decomp.h"
#include "Bs/dBsScene.h"

// RTTI 6ScBoot @ 0x008CD308
// vtable 0x008F8CA4 (vptr 0x008F8CAC), offset_to_top 0, 17 entries
class ScBoot : public ::BsScene
{
public:
    ScBoot(); // ctor candidate(s) 0x007F2F80 (unverified)
    virtual ~ScBoot(); // 0x0011448F slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00114483 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00114435 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x0011447F slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void CanCalc() const; // 0x00114411 slot 0x20 | slot vf_0x20 of oml::framework::Process
    virtual void Calc(); // 0x00114471 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00114427 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00758BD8 slot 0x40 | virtual slot, introduced by BsScene
};
