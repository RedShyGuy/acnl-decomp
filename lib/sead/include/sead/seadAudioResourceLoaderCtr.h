#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadAudioResourceLoader.h"

namespace sead {
// RTTI N4sead22AudioResourceLoaderCtrE @ 0x008D1EF0
// vtable 0x0090668C (vptr 0x00906694), offset_to_top 0, 7 entries
class AudioResourceLoaderCtr : public ::sead::AudioResourceLoader
{
public:
    AudioResourceLoaderCtr(); // ctor address unknown
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074DC94 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioResourceLoader::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074DC48 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983544 (guard 0x00983540)
        static const RuntimeTypeInfo::Derive<AudioResourceLoader> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x0013F6CC slot 0x08 | virtual slot, introduced by sead::AudioResourceLoaderCtr
    virtual void vf_0x0C(); // 0x0054B96C slot 0x0C | virtual slot, introduced by sead::AudioResourceLoaderCtr
    virtual void vf_0x10(); // 0x0054B744 slot 0x10 | virtual slot, introduced by sead::AudioResourceLoaderCtr
    virtual void vf_0x14(); // 0x0054B84C slot 0x14 | virtual slot, introduced by sead::AudioResourceLoaderCtr
    virtual void vf_0x18(); // 0x0054B968 slot 0x18 | virtual slot, introduced by sead::AudioResourceLoaderCtr
};
} // namespace sead
