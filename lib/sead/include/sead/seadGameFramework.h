#pragma once

#include "decomp.h"
#include "sead/RuntimeTypeInfo/seadDerive.h"
#include "sead/seadFramework.h"
#include "sead/seadSafeStringBase.h"

namespace sead {
class Thread;
class TickSpan;

// RTTI N4sead13GameFrameworkE @ 0x008D1668
// vtable 0x0090557C (vptr 0x00905584), offset_to_top 0, 23 entries
// The framework of a game: creates the system tasks below the root task and waits for the first
// task before the display loop (member names and the unnamed slots are ours).
class GameFramework : public ::sead::Framework
{
public:
    GameFramework(); // 0x0012C7B0
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x0074B89C (name is ours)
    {
        if (typeInfo == getTypeInfoStatic()) {
            return true;
        }
        return Framework::isKindOf(typeInfo);
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0074B850 (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x0098343C (guard 0x00983438)
        static const RuntimeTypeInfo::Derive<Framework> s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~GameFramework(); // 0x00543170 slot 0x08
    // 0x00543110 slot 0x0C (deleting dtor)
    virtual void createSystemTasks(TaskBase* rootTask, const CreateSystemTaskArg& arg); // 0x00542ED4 slot 0x14
    virtual void createControllerMgr(TaskBase* rootTask); // 0x00542F78 slot 0x38 | nintendogs:callseq
    virtual void vf_0x3C(TaskBase* rootTask, u32 arg0, u32 arg1); // 0x00542ECC slot 0x3C
    // the process meter on the primary heap of the root task (name is ours)
    virtual void createProcessMeter(TaskBase* rootTask); // 0x0054102C slot 0x40
    virtual void vf_0x44(TaskBase* rootTask); // 0x00542ED0 slot 0x44
    virtual void vf_0x48(TaskBase* rootTask, const TickSpan& span, s32 stackSize); // 0x00543040 slot 0x48
    // frames per second of the last frame (name is ours)
    virtual f32 calcFps() const = 0; // slot 0x4C
    virtual void vf_0x50(); // 0x00542EC8 slot 0x50
    virtual u32 vf_0x54() const { return 0; } // 0x0074B848 slot 0x54
    // creates the tasks until there is a root task (or the display was started) and starts the trees
    virtual void waitStartDisplayLoop_(); // 0x00543048 slot 0x58 | nintendogs:bytes-fuzzy

    // the lock function of the drawing: the draw lock of the graphics (name is ours)
    static void lockDrawContext_(bool lock); // 0x00132BB8

    // 0 before the display, 1 started by waitStartDisplayLoop_, 2 the LCDs are on
    u8 mDisplayState;                         // 0x28
    SafeStringBase<char> mString0;            // 0x2C
    SafeStringBase<char> mString1;            // 0x34
    SafeStringBase<char> mString2;            // 0x3C
    Thread* mThread;                          // 0x44, quit and deleted with the framework
    void (*mDrawLockFunction)(bool lock);     // 0x48
    u32 field_0x4C;                           // 0x4C
};
ASSERT_SIZE(GameFramework, 0x50);
} // namespace sead
