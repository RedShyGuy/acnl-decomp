#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadGameFrameworkCtrNw4c.h"

namespace sead {
// RTTI N4sead29DoubleCmdGameFrameworkCtrNw4cE @ 0x008D1F84
// vtable 0x00906864 (vptr 0x0090686C), offset_to_top 0, 36 entries
// The framework with two command lists (one is filled while the other runs) and three display buffers
// per screen; the LCDs are started after the second frame. Members and unnamed slots are ours.
class DoubleCmdGameFrameworkCtrNw4c : public ::sead::GameFrameworkCtrNw4c
{
public:
    explicit DoubleCmdGameFrameworkCtrNw4c(const CreateArg& arg); // 0x00120A64
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074E45C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return GameFrameworkCtrNw4c::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074E410 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x00983584 (guard 0x00983580)
        static const RuntimeTypeInfo::Derive<GameFrameworkCtrNw4c> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~DoubleCmdGameFrameworkCtrNw4c(); // 0x0054DF60 slot 0x08
    // 0x0054DF1C slot 0x0C (deleting dtor)
    virtual void mainLoop_(); // 0x0054DEE4 slot 0x60 | slot vf_0x60 of sead::GameFrameworkCtrNw4c
    virtual void procFrame_(); // 0x0054DAF0 slot 0x64 | slot vf_0x64 of sead::GameFrameworkCtrNw4c
    virtual void presentTop_(); // 0x0054DC6C slot 0x70 | slot vf_0x70 of sead::GameFrameworkCtrNw4c
    virtual void presentBtm_(); // 0x0054DC34 slot 0x74 | slot vf_0x74 of sead::GameFrameworkCtrNw4c
    virtual void swapBuffer_(); // 0x0054DCA4 slot 0x78 | nintendogs:callseq
    virtual void waitForVBlank_(); // 0x0054DD50 slot 0x7C | slot vf_0x7C of sead::GameFrameworkCtrNw4c
    virtual void vf_0x88(); // 0x0054DEC8 slot 0x88
    // a command list is done (from cmdListCallback_)
    virtual void onCmdListDone_(s32 id); // 0x0054DECC slot 0x8C

    // the command list callback of nngx that the game sets (name is ours)
    static void cmdListCallback_(s32 id); // 0x0011EC1C

    // the framework (name is ours)
    static DoubleCmdGameFrameworkCtrNw4c* s_pInstance;

    GLuint mCmdLists[2];          // 0x128
    GLuint mTopDisplayBuffers3[3]; // 0x130
    GLuint mBtmDisplayBuffers3[3]; // 0x13C
    s32 mCmdListIndex;            // 0x148, the list that is filled
    s32 mFrameCount;              // 0x14C, up to 3
    s64 mCmdListDoneTick;         // 0x150
    bool mVBlankDone;             // 0x158, false while the frame is processed
};
} // namespace sead
