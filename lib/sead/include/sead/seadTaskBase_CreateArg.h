#pragma once

#include "decomp.h"
#include "sead/seadDelegateEvent.h"
#include "sead/seadHeapArray.h"
#include "sead/seadHeapMgr.h"
#include "sead/seadNew.h"
#include "sead/seadTaskBase.h"
#include "sead/seadTaskClassID.h"

namespace sead {
class FaderTaskBase;
class Heap;
class TaskParameter;

// how the heap of a task in one root heap is made (names are ours)
struct TaskHeapPolicy
{
    // (out of line for the array constructor of CreateArg)
    DECOMP_EMIT_COPY TaskHeapPolicy() // 0x0011E87C
        : parent(nullptr), size(0), sizeLeftInParent(0), adjustSlack(0), adjust(false), fromTail(false),
          dontCreate(false)
    {
    }

    Heap* parent;          // 0x00, NULL: the heap of the parent task (or of the task manager)
    u32 size;              // 0x04, 0: all of the parent but sizeLeftInParent
    u32 sizeLeftInParent;  // 0x08
    u32 adjustSlack;       // 0x0C, kept free when the heap is adjusted
    bool adjust;           // 0x10, the heap is adjusted after prepare
    bool fromTail;         // 0x11
    bool dontCreate;       // 0x12
};
ASSERT_SIZE(TaskHeapPolicy, 0x14);

// the heap policies of a task, one per root heap (names are ours)
struct TaskHeapPolicies
{
    // makes index the primary heap and creates no other heap (if index is a root heap)
    void setPrimaryIndex(s32 index)
    {
        s32 num = HeapMgr::s_RootHeaps.size();
        if (static_cast<u32>(index) < static_cast<u32>(num)) {
            for (s32 i = 0; i < num; i++) {
                policies[i].dontCreate = i != index;
            }
            primaryIndex = index;
        }
    }

    TaskHeapPolicy policies[4]; // 0x00
    s32 primaryIndex;           // 0x50
};
ASSERT_SIZE(TaskHeapPolicies, 0x54);

// what a task is created with (the class name is from the reference symbols, the members are ours)
class TaskBase::CreateArg
{
public:
    CreateArg(); // 0x00561418 | nintendogs:callseq-callee [tier A]
    explicit CreateArg(const TaskClassID& classID); // 0x0011D89C

    TaskClassID factory;                     // 0x00
    TaskHeapPolicies heapPolicies;           // 0x08
    TaskBase* parent;                        // 0x5C, NULL: the root task
    TaskParameter* parameter;                // 0x60
    FaderTaskBase* fader;                    // 0x64, a fader that creates the task (the root task only)
    u32 field_0x68;                          // 0x68
    TaskBase** createdTask;                  // 0x6C, set to the task when it is created
    DelegateEvent<TaskBase*>::Slot* createdSlot; // 0x70, called with the task when it is created
    s32 field_0x74;                          // 0x74 (-1)
    TaskBase::Tag tag;                       // 0x78
    void (*instanceCallback)(TaskBase* task); // 0x7C, called right after the construction
};
ASSERT_SIZE(TaskBase::CreateArg, 0x80);

// the arguments of a system task: its heaps are adjusted, only the primary one is created (name is ours)
class SystemTaskCreateArg : public TaskBase::CreateArg
{
public:
    explicit SystemTaskCreateArg(const TaskClassID& classID); // 0x005612C0
};

// the factory function of a task class: T on the primary heap of the task (name is ours; one copy
// per class)
template <typename T>
TaskBase* TaskFactory(const TaskConstructArg& arg)
{
    return new (arg.heapArray->getPrimaryHeap(), 4) T(arg);
}
} // namespace sead
