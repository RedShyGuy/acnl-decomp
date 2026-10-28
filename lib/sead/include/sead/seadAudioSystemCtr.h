#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadAudioSystem.h"

namespace sead {
// RTTI N4sead14AudioSystemCtrE @ 0x008D16B8
// vtable 0x009056E0 (vptr 0x009056E8), offset_to_top 0, 17 entries
// vtable 0x0090572C (vptr 0x00905734), offset_to_top -4, 1 entries
class AudioSystemCtr : public ::sead::AudioSystem, public ::sead::hostio::Node
{
public:
    AudioSystemCtr(); // ctor candidate(s) 0x0012C8A0 (unverified)
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074BC64 (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return AudioSystem::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074BC18 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098345C (guard 0x00983458)
        static const RuntimeTypeInfo::Derive<AudioSystem> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual void vf_0x08(); // 0x00544094 slot 0x08 | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x0C(); // 0x0054406C slot 0x0C | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x10(); // 0x00543A30 slot 0x10 | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x14(); // 0x00544018 slot 0x14 | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x18(); // 0x00543C80 slot 0x18 | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x1C(); // 0x0074BBE8 slot 0x1C | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x20(); // 0x00543B68 slot 0x20 | virtual slot, introduced by sead::AudioSystemCtr
    virtual void appendEffect(sead::AudioGlobal::AuxBus, sead::AudioFx*); // 0x00543CC0 slot 0x24 | slot vf_0x24 of sead::AudioSystemCtr
    virtual void clearEffect(sead::AudioGlobal::AuxBus, int); // 0x00543B4C slot 0x28 | nintendogs:bytes
    virtual void vf_0x2C(); // 0x00543EC0 slot 0x2C | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x30(); // 0x00543F1C slot 0x30 | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x34(); // 0x00543FA4 slot 0x34 | virtual slot, introduced by sead::AudioSystemCtr
    virtual void vf_0x38(); // 0x00543EE0 slot 0x38 | virtual slot, introduced by sead::AudioSystemCtr
    virtual void initializeDsp_(); // 0x00463584 slot 0x3C | nintendogs:bytes
    virtual void finalizeDsp_(); // 0x00351A3C slot 0x40 | slot vf_0x40 of sead::AudioSystemCtr
};
} // namespace sead
