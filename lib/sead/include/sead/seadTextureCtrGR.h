#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadTextureCtr.h"

namespace sead {
// RTTI N4sead12TextureCtrGRE @ 0x008D15C0
// vtable 0x00905258 (vptr 0x00905260), offset_to_top 0, 9 entries
class TextureCtrGR : public ::sead::TextureCtr
{
public:
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074AC80 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return TextureCtr::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074AC34 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833F4 (guard 0x009833F0)
        static const RuntimeTypeInfo::Derive<TextureCtr> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~TextureCtrGR(); // 0x0053FD80 slot 0x08
    // 0x00541308 slot 0x0C (deleting dtor)
    virtual u32 getWidth() const { return mWidth; } // 0x0074AD84 slot 0x10
    virtual u32 getHeight() const { return mHeight; } // 0x0074AD8C slot 0x14
    virtual u32 vf_0x18() const { return field_0x0C; } // 0x0074AC2C slot 0x18
    virtual u8 vf_0x1C() const { return field_0x10; } // 0x0074AC1C slot 0x1C
    virtual u8 vf_0x20() const { return field_0x11; } // 0x0074AC24 slot 0x20
    TextureCtrGR(); // 0x005412D4 | nintendogs:bytes [tier A]

    // (names are ours)
    u32 mWidth;      // 0x04
    u32 mHeight;     // 0x08
    u32 field_0x0C;  // 0x0C
    u8 field_0x10;   // 0x10
    u8 field_0x11;   // 0x11
};
} // namespace sead
