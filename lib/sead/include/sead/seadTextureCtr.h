#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadTexture.h"

namespace sead {
// RTTI N4sead10TextureCtrE @ 0x008D13DC
// vtable 0x00904F58 (vptr 0x00904F60), offset_to_top 0, 9 entries
class TextureCtr : public ::sead::Texture
{
public:
    TextureCtr(); // ctor candidate(s) 0x0053FD6C (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074A53C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Texture::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074A4F0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833BC (guard 0x009833B8)
        static const RuntimeTypeInfo::Derive<Texture> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0053FD84 slot 0x08 | virtual slot, introduced by sead::TextureCtr
    virtual void vf_0x0C(); // 0x0053FD7C slot 0x0C | virtual slot, introduced by sead::TextureCtr
    virtual void vf_0x10(); // 0x0011C12F slot 0x10 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x14(); // 0x0011C12F slot 0x14 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x18(); // 0x0011C12F slot 0x18 | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x1C(); // 0x0011C12F slot 0x1C | slot vf_0x00 of ChangeRentalBase
    virtual void vf_0x20(); // 0x0011C12F slot 0x20 | slot vf_0x00 of ChangeRentalBase
};
} // namespace sead
