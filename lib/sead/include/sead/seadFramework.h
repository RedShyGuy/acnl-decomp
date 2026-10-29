#pragma once

#include "decomp.h"
#include "sead/seadDelegateEvent.h"
#include "sead/seadFrameBuffer.h"
#include "sead/seadTaskBase.h"
#include "sead/seadTickSpan.h"
#include "sead/RuntimeTypeInfo/seadRoot.h"

namespace sead {
class Heap;
class MethodTreeMgr;
class TaskMgr;

// RTTI N4sead9FrameworkE @ 0x008D2374
// vtable 0x009070C0 (vptr 0x009070C8), offset_to_top 0, 14 entries
// The framework of the program: the method trees and the tasks (member names are ours).
class Framework
{
public:
    // how the task manager runs (names are ours)
    struct RunArg
    {
        u32 prepareStackSize;  // 0x0, 0: 32 KiB
        s32 preparePriority;   // 0x4, -1: 17
    };

    // what the system tasks are created with (names are ours; GameFramework passes the words to its
    // slots 0x3C and 0x48)
    struct CreateSystemTaskArg
    {
        CreateSystemTaskArg(); // 0x0056268C

        u32 field_0x00; // 0x00
        u32 field_0x04; // 0x04
        TickSpan span;  // 0x08
        s32 stackSize;  // 0x10 (4096)
    };

    Framework(); // 0x0013374C
    // whether the class is the one of typeInfo or derives from it
    virtual bool isKindOf(const RuntimeTypeInfo::Interface* typeInfo) const // 0x007508E8 (name is ours)
    {
        return typeInfo == getTypeInfoStatic();
    }
    virtual const RuntimeTypeInfo::Interface* getTypeInfo() const // 0x0075089C (name is ours)
    {
        return getTypeInfoStatic();
    }
    // the type information of the class (inline everywhere; name is ours)
    static const RuntimeTypeInfo::Interface* getTypeInfoStatic()
    {
        // 0x009835F4 (guard 0x009835F0)
        static const RuntimeTypeInfo::Root s_TypeInfo;
        return &s_TypeInfo;
    }
    virtual ~Framework(); // 0x005627B4 slot 0x08
    // 0x005627A4 slot 0x0C (deleting dtor)
    // creates the method tree manager and the task manager with the root task (names of the slots are
    // ours; runImpl_/createMethodTreeMgr_/procReset_ after the reference symbols)
    virtual void run(Heap* heap, const TaskBase::CreateArg& rootTaskArg, const RunArg& runArg); // 0x005626B0 slot 0x10
    virtual void createSystemTasks(TaskBase* rootTask, const CreateSystemTaskArg& arg); // 0x00562680 slot 0x14
    // the frame buffer the methods of a method type draw into
    virtual FrameBuffer* getMethodFrameBuffer(s32 methodType) const = 0; // slot 0x18
    virtual LogicalFrameBuffer* getMethodLogicalFrameBuffer(s32 methodType) const // 0x00750944 slot 0x1C
    {
        return getMethodFrameBuffer(methodType);
    }
    virtual bool vf_0x20(); // 0x00562684 slot 0x20
    // the framework destroys all tasks and creates the root task again in procReset_
    virtual void requestReset(void* parameter); // 0x00562670 slot 0x24
    virtual void initRun_(Heap* heap); // 0x0056279C slot 0x28
    virtual void runImpl_(); // 0x005627A0 slot 0x2C | slot vf_0x2C of sead::Framework
    virtual MethodTreeMgr* createMethodTreeMgr_(Heap* heap) = 0; // slot 0x30
    virtual void procReset_(); // 0x00562600 slot 0x34 | nintendogs:bytes

    bool mResetRequested;              // 0x04
    void* mResetParameter;             // 0x08
    DelegateEvent<void*> mResetEvent;  // 0x0C, told before the reset
    TaskMgr* mTaskMgr;                 // 0x1C
    MethodTreeMgr* mMethodTreeMgr;     // 0x20
    Heap* mMethodTreeMgrHeap;          // 0x24
};
ASSERT_SIZE(Framework, 0x28);
} // namespace sead
