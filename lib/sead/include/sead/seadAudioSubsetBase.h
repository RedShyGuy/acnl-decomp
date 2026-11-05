#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadListNode.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
class AudioMgr;
class Heap;

// RTTI N4sead15AudioSubsetBaseE @ 0x008D177C
// A part of the audio of the program that the AudioMgr runs (only the slots that sead calls; names are
// ours). The reset slots answer the AudioResetter.
class AudioSubsetBase : public ::sead::hostio::Node
{
public:
    AudioSubsetBase() : hostio::Node(), mNode() {}
    // (the runtime type info slots 0x04/0x08 as in AudioPlayer; names are ours)
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074C430 slot 0x04
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074C3E4 slot 0x08
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098348C (guard 0x00983488)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~AudioSubsetBase() {}
    // (the default bodies are empty; the copies 0x00545B68.. and 0x0074C3C4.. are folded across the slots)
    virtual void initialize(AudioMgr* mgr, Heap* heap) {} // 0x00545B68 slot 0x14
    virtual void finalize() {} // 0x00545B84 slot 0x18
    virtual void calc() = 0; // slot 0x1C
    virtual void vf_0x20() {} // 0x00545B70 slot 0x20
    virtual void vf_0x24() {} // 0x00545B74 slot 0x24
    virtual void startReset(int fadeFrames) {} // 0x00545B7C slot 0x28
    virtual bool isResetting() const { return false; } // 0x0074C3CC slot 0x2C
    virtual bool isResetReady() { return true; } // 0x0074C3C4 slot 0x30
    virtual void endReset() {} // 0x00545B6C slot 0x34
    virtual void startStop(int fadeFrames) {} // 0x00545B88 slot 0x38
    virtual bool isStopping() const { return false; } // 0x0074C3DC slot 0x3C
    virtual bool isStopReady() { return true; } // 0x0074C3D4 slot 0x40
    virtual void vf_0x44() {} // 0x00545B78 slot 0x44
    virtual void vf_0x48() {} // 0x00545B80 slot 0x48
    virtual s32 vf_0x4C() { return 0; } // 0x0074C48C slot 0x4C

    ListNode mNode; // 0x04, in the list of the AudioMgr
};
} // namespace sead
