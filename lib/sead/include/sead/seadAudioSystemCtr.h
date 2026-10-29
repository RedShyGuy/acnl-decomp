#pragma once

#include "decomp.h"
#include "nw/snd/snd_SoundSystem.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadAudioSystem.h"
#include "sead/seadCriticalSection.h"
#include "sead/seadListImpl.h"

namespace sead {
class Heap;

// RTTI N4sead14AudioSystemCtrE @ 0x008D16B8
// vtable 0x009056E0 (vptr 0x009056E8), offset_to_top 0, 17 entries
// vtable 0x0090572C (vptr 0x00905734), offset_to_top -4, 1 entries
// The sound system of the 3DS: the DSP, nn::snd and nw::snd::SoundSystem. With mIsSplitInitialize the
// driver commands come first and the sound system later (initializeSoundSystem_). The frame callbacks
// run in the user callback of the sound thread. Member names are ours.
class AudioSystemCtr : public ::sead::AudioSystem, public ::sead::hostio::Node
{
public:
    AudioSystemCtr(); // 0x0012C8A0
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
    virtual ~AudioSystemCtr(); // 0x00544094 slot 0x08
    // 0x0054406C slot 0x0C (deleting dtor)
    virtual void initialize(); // 0x00543A30 slot 0x10
    virtual void finalize(); // 0x00544018 slot 0x14
    virtual bool setOutputMode(AudioGlobal::OutputMode mode); // 0x00543C80 slot 0x18
    virtual AudioGlobal::OutputMode getOutputMode() const; // 0x0074BBE8 slot 0x1C
    virtual bool appendEffect(AudioGlobal::AuxBus bus, AudioFx* fx); // 0x00543B68 slot 0x20
    virtual bool appendEffect(AudioGlobal::AuxBus bus, AudioFxObject* fx); // 0x00543CC0 slot 0x24
    virtual void clearEffect(AudioGlobal::AuxBus bus, int fadeFrames); // 0x00543B4C slot 0x28 | nintendogs:bytes
    virtual bool isEffectFinalized(AudioGlobal::AuxBus bus); // 0x00543EC0 slot 0x2C
    virtual void addFrameCallback(FrameCallback* callback); // 0x00543F1C slot 0x30
    virtual void removeFrameCallback(FrameCallback* callback); // 0x00543FA4 slot 0x34
    virtual void clearFrameCallbacks(); // 0x00543EE0 slot 0x38
    virtual void initializeDsp_(); // 0x00463584 slot 0x3C | nintendogs:bytes
    virtual void finalizeDsp_(); // 0x00351A3C slot 0x40

    // the second step of the split initialization: the DSP and the sound system
    void initializeSoundSystem_(); // 0x00543DFC
    // the user callback of the sound thread: calls the frame callbacks
    static void soundFrameCallback_(uptr arg); // 0x00543E58

    s32 mSoundThreadPriority;          // 0x08
    s32 mTaskThreadPriority;           // 0x0C
    u32 mSoundThreadStackSize;         // 0x10
    u32 mTaskThreadStackSize;          // 0x14
    Heap* mHeap;                       // 0x18
    void* mWorkMem;                    // 0x1C, of the sound system
    u32 mWorkMemSize;                  // 0x20
    u8 mSoundThreadCoreNo;             // 0x24
    bool mEnableSoundThread;           // 0x25
    bool mEnableGetSoundThreadTick;    // 0x26
    s32 mSoundThreadProfileCount;      // 0x28
    bool mEnableUserThread;            // 0x2C
    nw::snd::SoundSystem::SoundSystemParam mParam; // 0x30
    bool mIsInitialized;               // 0x54
    void* mDriverCommandMem;           // 0x58
    bool mIsSplitInitialize;           // 0x5C
    bool mIsSoundSystemInitialized;    // 0x5D
    OffsetList<FrameCallback> mFrameCallbacks; // 0x60
    CriticalSection mCriticalSection;  // 0x70
};
} // namespace sead
