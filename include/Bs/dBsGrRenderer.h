#pragma once

#include "decomp.h"
#include "Other/dBase.h"
#include "Utl/dUtlBase.h"

// RTTI 12BsGrRenderer @ 0x008CB520
// vtable 0x008EE044 (vptr 0x008EE04C), offset_to_top 0, 22 entries
class BsGrRenderer : public ::UtlBase<Base>
{
public:
    class Functor;
    BsGrRenderer(); // ctor candidate(s) 0x001F9C88 (unverified)
    virtual ~BsGrRenderer(); // 0x001F9D24 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x001F9CF8 slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x0082067C slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00820718 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x001F9C80 slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x001F9C68 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x001F9B68 slot 0x40 | virtual slot, introduced by BsGrRenderer
    virtual void vf_0x44(); // 0x001F9B60 slot 0x44 | virtual slot, introduced by BsGrRenderer
    virtual void vf_0x48(); // 0x001F9B10 slot 0x48 | virtual slot, introduced by BsGrRenderer
    virtual void vf_0x4C(); // 0x001F9C58 slot 0x4C | virtual slot, introduced by BsGrRenderer
    virtual void vf_0x50(); // 0x001F9C50 slot 0x50 | virtual slot, introduced by BsGrRenderer
    virtual void vf_0x54(); // 0x001F9BC8 slot 0x54 | virtual slot, introduced by BsGrRenderer
};
