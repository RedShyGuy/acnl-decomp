#pragma once

#include "decomp.h"
#include "sead/seadTexture.h"

namespace sead {
// RTTI N4sead10TextureCtrE @ 0x008D13DC
// vtable 0x00904F58 (vptr 0x00904F60), offset_to_top 0, 9 entries
class TextureCtr : public ::sead::Texture
{
public:
    TextureCtr(); // ctor candidate(s) 0x0053FD6C (unverified)
    virtual void vf_0x00(); // 0x0074A53C slot 0x00 | virtual slot, introduced by sead::TextureCtr
    virtual void vf_0x04(); // 0x0074A4F0 slot 0x04 | virtual slot, introduced by sead::TextureCtr
    virtual void vf_0x08(); // 0x0053FD84 slot 0x08 | virtual slot, introduced by sead::TextureCtr
    virtual void vf_0x0C(); // 0x0053FD7C slot 0x0C | virtual slot, introduced by sead::TextureCtr
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
