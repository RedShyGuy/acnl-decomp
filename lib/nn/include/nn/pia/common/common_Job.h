#pragma once

#include "decomp.h"
#include "nn/pia/common/common_ListBase.h"
#include "nn/pia/common/common_RootObject.h"
#include "nn/pia/common/common_Time.h"

namespace nn {
namespace pia {
namespace common {
// What ExecuteCore asks the scheduler to do next (returned in r0: the state in the low byte, a
// wait time in the high half). Job::Execute switches on it; the names are ours.
struct ExecuteResult
{
    enum State : u8
    {
        STATE_CONTINUE = 0,            // execute again in the same scheduler at once
        STATE_SUCCESS = 1,             // done
        STATE_FAILURE = 2,             // done (StepSequenceJob returns it when canceled)
        STATE_SUSPEND = 3,             // wait for Job::Resume
        STATE_WAIT = 4,                // execute again after m_WaitMSec
        STATE_NEXT_DISPATCH = 5,       // execute again in the next Scheduler::Dispatch
        STATE_CONTINUE_FOREGROUND = 6, // execute again at once, in the foreground
        STATE_CONTINUE_BACKGROUND = 7, // execute again at once, in the background thread
    };

    ExecuteResult(State state, u16 waitMSec = 0) : m_State(state), m_WaitMSec(waitMSec) {}

    State m_State;    // 0x0
    u16 m_WaitMSec;   // 0x2
};
ASSERT_SIZE(ExecuteResult, 0x4);

// RTTI N2nn3pia6common3JobE @ 0x008CFEC4
// vtable 0x009015F0 (vptr 0x009015F8), offset_to_top 0, 4 entries
//
// A task that the Scheduler executes in its Dispatch (foreground) or in the thread of the
// BackgroundScheduler. All state changes happen under the lock of the Scheduler. The member
// names and the state names are ours.
class Job : public ::nn::pia::common::RootObject
{
public:
    // m_State
    enum State : u8
    {
        STATE_IDLE = 0,
        STATE_WAITING = 1,              // foreground, waits for its time
        STATE_READY = 2,                // foreground, queued
        STATE_RUNNING = 3,              // foreground, in ExecuteCore
        STATE_BACKGROUND_WAITING = 4,
        STATE_BACKGROUND_READY = 5,
        STATE_BACKGROUND_RUNNING = 6,
        STATE_SUSPENDED = 7,
        STATE_FINISHED = 8,
    };

    // GetState (the public view of m_State)
    enum ExecuteState
    {
        EXECUTE_STATE_IDLE = 0,
        EXECUTE_STATE_WAITING = 1,
        EXECUTE_STATE_SUSPENDED = 2,
        EXECUTE_STATE_READY = 3,
        EXECUTE_STATE_RUNNING = 4,
        EXECUTE_STATE_FINISHED = 5,
    };

    Job(); // 0x00428B74 | fefates:bytes [tier B]
    virtual ~Job(); // 0x00428BA8 slot 0x00 | fefates:callgraph
    // 0x00428BA0 slot 0x04 (deleting dtor)
    virtual void Reset(bool clearAll); // 0x00428934 slot 0x08 | fefates:bytes-fuzzy
    virtual ExecuteResult ExecuteCore() = 0; // 0x0011C12F slot 0x0C

    // queue the job in the foreground or background (from idle / suspended); the parameter
    // name is ours
    void Ready(bool background); // 0x004288C4 | fefates:bytes [tier B]
    void Resume(bool background); // 0x004289A4 | fefates:bytes [tier B]
    // called by the schedulers with their lock held
    void Execute(bool background); // 0x00428A14 | fefates:bytes-fuzzy [tier B]

    bool IsBackground() const; // 0x00733184 | fefates:callgraph [tier C]
    bool IsForeground() const; // 0x007331F0 | fefates:bytes-fuzzy [tier B]
    ExecuteState GetState() const; // 0x00733250 | fefates:callgraph [tier C]
    bool IsRunning() const; // 0x00733304 | fefates:callgraph [tier C]

    State m_State;           // 0x04
    u32 m_ExecuteCount;      // 0x08, ExecuteCore calls since Reset
    Time m_ExecuteTime;      // 0x10, when to execute it
    ListNode m_ListNode;     // 0x18, node in the lists of the schedulers
};
ASSERT_SIZE(Job, 0x20);
ASSERT_OFFSET(Job, m_ListNode, 0x18);
} // namespace common
} // namespace pia
} // namespace nn
