#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadAudioGlobal.h"
#include "sead/seadListNode.h"

namespace sead {
class AudioFxHolder;
class AudioFxMemoryMgr;

// RTTI N4sead7AudioFxE @ 0x008D2130
// vtable 0x00906BE8 (vptr 0x00906BF0), offset_to_top 0, 4 entries
// An effect of the sound: its holder makes the effect of the sound library, AudioFxMgr gives it memory
// and a bus (member names are ours).
class AudioFx
{
public:
    AudioFx(); // 0x001391DC | nintendogs:bytes [tier B]
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074EECC (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074EE80 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835B4 (guard 0x009835B0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioFx(); // 0x0014397C slot 0x08
    // 0x0055DF98 slot 0x0C (deleting dtor)

    AudioFxMemoryMgr* mMemoryMgr; // 0x04, the memory of the work buffer (set by AudioFxMgr::setup)
    AudioFxHolder* mHolder;       // 0x08
    AudioGlobal::AuxBus mBus;     // 0x0C
    ListNode mNode;               // 0x10, in AudioFxMgr
};
ASSERT_SIZE(AudioFx, 0x18);
} // namespace sead
