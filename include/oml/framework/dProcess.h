#pragma once

#include "decomp.h"

namespace oml {
namespace framework {
// the result of a step of a process (names after libgarden)
enum Result : u8
{
    RESULT_INVALID = 0, // the step could not run (Can... returned false)
    RESULT_FAIL = 1,    // the process is stopped
    RESULT_SUCCESS = 2,
    RESULT_RETRY = 3,   // the step runs again next time
};

// the index of a process class in the tables of the ProcessManager (name after libgarden)
typedef u16 ProcessIndex;

// A process of the framework of the game: made by the ProcessManager from a class index, it runs through the steps
// Initialize, then Calc and Draw every frame, and Finalize after it was stopped (class, slot and some member names
// after libgarden and the reference symbols; the others are ours).
// RTTI N3oml9framework7ProcessE @ 0x008D1028
// vtable 0x0090476C (vptr 0x00904774), offset_to_top 0, 15 entries
class Process
{
public:
    // m_State
    enum State : u8
    {
        STATE_CREATED = 0,  // waiting for Initialize to succeed
        STATE_RUNNING = 1,  // in the calc and draw lists
        STATE_STOPPING = 2, // waiting for Finalize to succeed
    };

    Process(); // 0x00522138 | libgarden [tier A]
    virtual ~Process(); // 0x0052218C slot 0x00 | libgarden
    // 0x00522184 (deleting dtor)
    virtual bool CanInitialize() const; // 0x00522128 slot 0x08
    virtual Result Initialize(); // 0x005220BC slot 0x0C
    virtual void HandleInitializationResult(Result result); // 0x00521614 slot 0x10
    virtual bool CanFinalize() const; // 0x00522130 slot 0x14
    virtual Result Finalize(); // 0x005220DC slot 0x18
    virtual void HandleFinalizationResult(Result result); // 0x0052161C slot 0x1C
    virtual bool CanCalc() const; // 0x00521620 slot 0x20
    virtual Result Calc(); // 0x005220C8 slot 0x24
    virtual void HandleCalcResult(Result result); // 0x00521718 slot 0x28
    virtual bool CanDraw() const; // 0x005220D0 slot 0x2C
    virtual Result Draw(); // 0x005220B0 slot 0x30
    virtual void ProcessDrawResult(Result result); // 0x005220E8 slot 0x34 | libgarden
    // the process is being stopped (name after the reference symbols; libgarden: OnStop)
    virtual void OnNotify(); // 0x00521710 slot 0x38 | libgarden

    // the process and its children are stopped (they finalize at the next update)
    void Stop(); // 0x00521754 | libgarden [tier A]
    void StopSelf(); // 0x00520B5C | libgarden [tier A]
    // the factory of a process class (name after libgarden; the instances are in the code of the classes)
    template <typename T>
    static Process* Create()
    {
        return new T();
    }
    // destroys a process (deleting destructor)
    static void Destroy(Process* process); // 0x005251F0 | libgarden [tier A]
    // memory of the nothrow operator new[], cleared
    static void* operator new(size_t size); // 0x0052A488 | libgarden [tier A]
    static void operator delete(void* p); // 0x00313B24 | libgarden [tier A]

    // (names are ours) the class index, the argument and the type of the ProcessManager
    DECOMP_NOIPA void Set(ProcessIndex index, u32 arg, u8 type); // 0x0052176C
    void SetArg(u32 arg); // 0x005220EC

    // (inline; copies of the original)
    DECOMP_EMIT_COPY DECOMP_NOINLINE void SetState(u8 state) { m_State = state; } // 0x0052209C
    DECOMP_EMIT_COPY DECOMP_NOINLINE void SetActivationPending(bool isPending) { m_IsActivationPending = isPending; } // 0x005220A4
    DECOMP_EMIT_COPY s32 GetHandle() const { return m_Handle; } // 0x007487E4
    DECOMP_EMIT_COPY u8 GetState() const { return m_State; } // 0x007487EC
    DECOMP_EMIT_COPY bool IsStopped() const { return m_IsStopped; } // 0x007487F4
    DECOMP_EMIT_COPY u32 GetArg() const { return m_Arg; } // 0x007487FC

    s32 m_Handle;                       // 0x04 (name after libgarden) the slot in the low 10 bits
    u32 m_Arg;                          // 0x08 given to CreateProcess
    ProcessIndex m_Index;               // 0x0C (name after libgarden) the class
    u8 m_State;                         // 0x0E (name after libgarden) State
    bool m_IsStopped;                   // 0x0F (name after libgarden) stopped, not yet in the finalize list
    bool m_IsActivationPending;         // 0x10 initialized during the calc step: into the lists at the next update
    bool m_IsInitializationDeferred;    // 0x11 made during the initialization step: into the list at the next update
    u8 m_Type;                          // 0x12 given to CreateProcess (libgarden: m_Param2)
};
ASSERT_SIZE(Process, 0x14);
} // namespace framework
} // namespace oml
