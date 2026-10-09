#pragma once

#include "decomp.h"

namespace nn {
namespace applet {
namespace CTR {
// A callback that the applet library calls (with its argument, in a list under a lock) when the
// system has accepted to sleep. Layout from Register / Unregister and the call at 0x00124B00; the
// constructor is inline (in the static initializer of the dsp library); the member names are ours.
class SysSleepAcceptedCallbackInfo
{
public:
    typedef void (*Callback)(uptr argument);

    SysSleepAcceptedCallbackInfo(Callback callback, uptr argument, s32 priority)
        : m_pPrev(0), m_pNext(0), m_Callback(callback), m_Argument(argument), m_Priority(priority)
    {
    }

    void Unregister(); // 0x0047FD28 | nintendogs:bytes [tier A]
    void Register(); // 0x0047FDA8 | nintendogs:bytes [tier A]

    SysSleepAcceptedCallbackInfo* m_pPrev; // 0x00
    SysSleepAcceptedCallbackInfo* m_pNext; // 0x04
    Callback m_Callback;                   // 0x08
    uptr m_Argument;                       // 0x0C
    s32 m_Priority;                        // 0x10, the list is sorted by it
};
ASSERT_SIZE(SysSleepAcceptedCallbackInfo, 0x14);
} // namespace CTR
} // namespace applet
} // namespace nn
