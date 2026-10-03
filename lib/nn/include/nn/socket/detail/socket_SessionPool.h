#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/fnd/fnd_IntrusiveLinkedList.h"
#include "nn/fnd/fnd_TimeSpan.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_LightEvent.h"
#include "nn/os/os_Thread.h"
#include "nn/socket/detail/socket_SessionItem.h"

#include <string.h>

namespace nn {
namespace socket {
namespace detail {
// results of the pool (module socket, 28; names are ours)
const bit32 RESULT_NOT_INITIALIZED = 0xC8A073F8;        // status, invalid state, 1016
const bit32 RESULT_ALREADY_INITIALIZED = 0xC8A073F9;    // status, invalid state, 1017
const bit32 RESULT_NO_FREE_SESSION = 0xD8607008;        // permanent, out of resource, 8
const bit32 DESCRIPTION_NO_FREE_SESSION = 8;

// RTTI N2nn6socket6detail11SessionPoolE @ 0x008D04F8
// vtable 0x0090231C (vptr 0x00902324), offset_to_top 0, 1 entries
//
// Sessions of one service (soc:U) for the calls that may block: a call takes an idle session
// (making a new one from a free item if there is none) and gives it back afterwards. Items whose
// session was closed go back to the free list. The members and the inline functions are ours.
class SessionPool
{
public:
    // inline (in __sti___21_socket_IpcWrapper_cpp)
    SessionPool()
        : mLock(nn::os::CriticalSection::InitializeTag()), mBusyCount(0), mFreeCount(0), mIdleCount(0),
          mUnknown30(0), mIsInitialized(false), mIsFinalizing(false)
    {
        mEvent.Initialize(false);
    }
    // inline (in ~SessionPoolAuto); not virtual
    ~SessionPool() { Finalize(); }

    // called for each new session (name is ours)
    virtual nn::Result InitializeSession(nn::os::ipc::Session* session) { return nn::Result(); } // 0x00485A9C slot 0x00

    // closes the idle sessions and drops the free items; the busy ones stay
    void SemiFinalize(); // 0x00485888 | fefates:bytes [tier B]
    // a session from a free item into the idle list
    nn::Result AddNewSession(); // 0x00485994 | fefates:bytes [tier B]

    // inline (names are ours)
    nn::Result Initialize(SessionItem* items, s32 count, const char* name, size_t nameLength);
    void Finalize();

    // takes an idle session (inline, in the functions of socket_IpcWrapper.cpp)
    nn::Result Acquire(SessionItem** item)
    {
        nn::os::CriticalSection::ScopedLock lock(mLock);
        if (!mIsInitialized || mIsFinalizing) {
            return nn::Result(RESULT_NOT_INITIALIZED);
        }
        if (mIdleSessions.IsEmpty()) {
            nn::Result result = AddNewSession();
            if (result.IsFailure()) {
                return result;
            }
        }
        SessionItem* taken = mIdleSessions.PopFront();
        mIdleCount--;
        mBusySessions.PushBack(taken);
        mBusyCount++;
        *item = taken;
        return nn::Result();
    }

    // gives a session back (inline)
    void Release(SessionItem* item)
    {
        nn::os::CriticalSection::ScopedLock lock(mLock);
        if (!mIsInitialized) {
            return;
        }
        mBusySessions.Erase(item);
        mBusyCount--;
        if (item->mSession.GetHandle().IsValid()) {
            mIdleSessions.PushBack(item);
            mIdleCount++;
        } else {
            mFreeItems.PushBack(item);
            mFreeCount++;
        }
        mEvent.Signal();
    }

    // waits until a session is given back (inline)
    void WaitForRelease() { mEvent.Wait(); }

protected:
    typedef nn::fnd::IntrusiveLinkedList<SessionItem, void> SessionList;

    SessionList mBusySessions;      // 0x04 in use
    SessionList mIdleSessions;      // 0x08 open, not in use
    SessionList mFreeItems;         // 0x0C without a session
    nn::os::CriticalSection mLock;  // 0x10
    nn::os::LightEvent mEvent;      // 0x1C signalled when a session is given back
    s32 mBusyCount;                 // 0x24
    s32 mFreeCount;                 // 0x28
    s32 mIdleCount;                 // 0x2C
    s32 mUnknown30;                 // 0x30 only set to 0
    size_t mNameLength;             // 0x34
    char mName[9];                  // 0x38 the service
    bool mIsInitialized;            // 0x41
    bool mIsFinalizing;             // 0x42
};
ASSERT_SIZE(SessionPool, 0x44);

// a time the pool waits for the busy sessions while it is finalized
const s64 FINALIZE_POLL_MILLISECONDS = 100;

inline nn::Result SessionPool::Initialize(SessionItem* items, s32 count, const char* name, size_t nameLength)
{
    nn::os::CriticalSection::ScopedLock lock(mLock);
    if (mIsInitialized) {
        return nn::Result(RESULT_ALREADY_INITIALIZED);
    }
    strlcpy(mName, name, sizeof(mName));
    mNameLength = nameLength;
    for (s32 i = 0; i < count; i++) {
        mFreeItems.PushBack(&items[i]);
        mFreeCount++;
    }
    nn::Result result = AddNewSession();
    if (result.IsFailure()) {
        mLock.Exit();
        Finalize();
        mLock.Enter();
        return result;
    }
    mIsInitialized = true;
    mIsFinalizing = false;
    return nn::Result();
}

inline void SessionPool::Finalize()
{
    SemiFinalize();
    {
        nn::os::CriticalSection::ScopedLock lock(mLock);
        for (SessionItem* item = mBusySessions.GetFront(); item != 0; item = mBusySessions.GetNext(item)) {
            item->mSession.Close();
        }
        while (mBusyCount != 0) {
            mLock.Exit();
            nn::os::Thread::SleepImpl(nn::fnd::TimeSpan::FromMilliSeconds(FINALIZE_POLL_MILLISECONDS));
            mLock.Enter();
        }
        mIsInitialized = false;
    }
    SemiFinalize();
}
} // namespace detail
} // namespace socket
} // namespace nn
