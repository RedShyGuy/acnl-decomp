#pragma once

#include "decomp.h"
#include "sead/hostio/seadNode.h"
#include "sead/seadListNode.h"

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
    virtual ~AudioSubsetBase() {}
    virtual void vf_0x0C() = 0;
    virtual void vf_0x10() = 0;
    virtual void initialize(AudioMgr* mgr, Heap* heap) = 0; // slot 0x14
    virtual void finalize() = 0; // slot 0x18
    virtual void calc() = 0; // slot 0x1C
    virtual void vf_0x20() = 0;
    virtual void vf_0x24() = 0;
    virtual void startReset(int fadeFrames) = 0; // slot 0x28
    virtual bool isResetting() const = 0; // slot 0x2C
    virtual bool isResetReady() = 0; // slot 0x30
    virtual void endReset() = 0; // slot 0x34
    virtual void startStop(int fadeFrames) = 0; // slot 0x38
    virtual bool isStopping() const = 0; // slot 0x3C
    virtual bool isStopReady() = 0; // slot 0x40

    ListNode mNode; // 0x04, in the list of the AudioMgr
};
} // namespace sead
