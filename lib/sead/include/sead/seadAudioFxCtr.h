#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioFx.h"

namespace sead {
// RTTI N4sead10AudioFxCtrE @ 0x008D134C
// vtable 0x00904DF4 (vptr 0x00904DFC), offset_to_top 0, 4 entries
class AudioFxCtr : public ::sead::AudioFx
{
public:
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x00749E14 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioFx::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x00749DA0 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098339C (guard 0x00983398)
        static const RuntimeTypeInfo::Derive<AudioFx> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x00141B98 slot 0x08 | virtual slot, introduced by sead::AudioFx
    virtual void vf_0x0C(); // 0x0053E214 slot 0x0C | nintendogs:bytes
    AudioFxCtr(); // 0x00132CEC | nintendogs:bytes [tier B]
};
} // namespace sead
