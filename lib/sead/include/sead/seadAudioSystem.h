#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"
#include "sead/seadAudioGlobal.h"
#include "sead/seadListNode.h"

namespace sead {
class AudioFx;
class AudioFxObject;

// RTTI N4sead11AudioSystemE @ 0x008D13F0
// The sound system: the sound library, its effects and the callbacks of the sound frames (an interface;
// the names of the slots are ours except appendEffect/clearEffect/initializeDsp_/finalizeDsp_).
class AudioSystem
{
public:
    // a callback of each sound frame (in the sound thread; name is ours)
    class FrameCallback
    {
    public:
        virtual void invoke() = 0; // slot 0x00

        ListNode mNode; // 0x4
    };

    AudioSystem() {}
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009833CC (guard 0x009833C8)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioSystem() {}
    virtual void initialize() = 0; // slot 0x10
    virtual void finalize() = 0; // slot 0x14
    virtual bool setOutputMode(AudioGlobal::OutputMode mode) = 0; // slot 0x18
    virtual AudioGlobal::OutputMode getOutputMode() const = 0; // slot 0x1C
    virtual bool appendEffect(AudioGlobal::AuxBus bus, AudioFx* fx) = 0; // slot 0x20
    virtual bool appendEffect(AudioGlobal::AuxBus bus, AudioFxObject* fx) = 0; // slot 0x24
    virtual void clearEffect(AudioGlobal::AuxBus bus, int fadeFrames) = 0; // slot 0x28
    virtual bool isEffectFinalized(AudioGlobal::AuxBus bus) = 0; // slot 0x2C
    virtual void addFrameCallback(FrameCallback* callback) = 0; // slot 0x30
    virtual void removeFrameCallback(FrameCallback* callback) = 0; // slot 0x34
    virtual void clearFrameCallbacks() = 0; // slot 0x38
    virtual void initializeDsp_() = 0; // slot 0x3C
    virtual void finalizeDsp_() = 0; // slot 0x40
};
} // namespace sead
