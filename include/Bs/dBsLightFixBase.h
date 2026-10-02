#pragma once

#include "decomp.h"
#include "Bs/dBsLightBase.h"

// RTTI 14BsLightFixBase @ 0x008CBB98
// vtable 0x008EFCCC (vptr 0x008EFCD4), offset_to_top 0, 21 entries
class BsLightFixBase : public ::BsLightBase
{
public:
    class FixLightHioNode;
    BsLightFixBase(); // ctor address unknown
    virtual ~BsLightFixBase(); // 0x00256BF0 slot 0x00 | slot vf_0x00 of oml::framework::Process
    // 0x00256BCC slot 0x04 | slot vf_0x04 of oml::framework::Process (deleting dtor)
    virtual void Initialize(); // 0x00256994 slot 0x0C | slot vf_0x0C of oml::framework::Process
    virtual void Finalize(); // 0x00256B08 slot 0x18 | slot vf_0x18 of oml::framework::Process
    virtual void Calc(); // 0x00256ACC slot 0x24 | slot vf_0x24 of oml::framework::Process
    virtual void Draw(); // 0x00256948 slot 0x30 | slot vf_0x30 of oml::framework::Process
    virtual void vf_0x40(); // 0x00719264 slot 0x40 | virtual slot, introduced by BsLightBase
    virtual void vf_0x44(); // 0x00256B28 slot 0x44 | virtual slot, introduced by BsLightBase
    virtual void vf_0x48(); // 0x00256AF8 slot 0x48 | virtual slot, introduced by BsLightBase
    virtual void vf_0x4C(); // 0x00256868 slot 0x4C | virtual slot, introduced by BsLightFixBase
    virtual void vf_0x50(); // 0x00256850 slot 0x50 | virtual slot, introduced by BsLightFixBase
};
