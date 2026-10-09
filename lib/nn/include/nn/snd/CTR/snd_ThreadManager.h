#pragma once

#include "decomp.h"
#include "nn/Result.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_LightEvent.h"
#include "nn/os/os_Thread.h"
#include "nn/os/os_Tick.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
// The sound thread: waits for each frame of the DSP, calls the callbacks and sends the
// parameters. With the sound thread on the system core (1), the user callback and the aux
// callbacks run on a second thread. Member names are ours.
class ThreadManager
{
public:
    typedef void (*Callback)(uptr arg);

    ThreadManager();
    ~ThreadManager(); // 0x00462594 | nintendogs:bytes [confirmed by fefates] [tier A]

    static ThreadManager* GetInstance(); // 0x00461F60 | nintendogs:callgraph [confirmed by fefates] [tier A]
    nn::Result StartSoundThread(Callback userCallback, uptr userArg, uptr stackBuffer, u32 stackSize, s32 priority, s32 coreNo); // 0x00462008 | nintendogs:callgraph [confirmed by fefates] [tier A]
    nn::Result StartSoundThread(const ThreadParameter* parameter, Callback callback, uptr arg, const ThreadParameter* userParameter,
                                Callback userCallback, uptr userArg, int coreNo); // 0x0046212C | nintendogs:bytes [confirmed by fefates] [tier A]
    nn::os::Tick GetSoundThreadTick() const; // 0x004621D8 (name is ours)
    void FinalizeSoundThread(); // 0x004621F4 | nintendogs:callgraph [confirmed by fefates] [tier A]
    static void SoundThreadFunc(uptr arg); // 0x004622D8 (name is ours)
    void SoundThreadFuncImpl(uptr arg); // 0x004622F0 | fefates:bytes [tier B]
    nn::Result StartUserSoundThread(uptr stackBuffer, u32 stackSize, s32 priority); // 0x00462474 | nintendogs:bytes [confirmed by fefates] [tier A]
    void EnableSoundThreadTickCounter(bool isEnabled); // 0x00462584 (name is ours)

    s64 m_Tick;                             // 0x00, the time of the last frame
    bool m_IsTickCounterEnabled;            // 0x08
    u8 m_Padding9[3];                       // 0x09
    bool m_IsStarted;                       // 0x0C
    bool m_IsRunning;                       // 0x0D
    bool m_IsUserThreadStarted;             // 0x0E
    bool m_IsUserThreadRunning;             // 0x0F
    nn::os::Thread m_Thread;                // 0x10
    nn::os::Thread m_UserThread;            // 0x18
    Callback m_Callback;                    // 0x20, under m_Lock
    uptr m_CallbackArg;                     // 0x24
    Callback m_UserCallback;                // 0x28
    uptr m_UserCallbackArg;                 // 0x2C
    u8 m_CoreNo;                            // 0x30
    nn::os::CriticalSection m_Lock;         // 0x34
    nn::os::LightEvent m_UserThreadEvent;   // 0x40, a frame for the user thread
    nn::os::LightEvent m_DoneEvent;         // 0x48, the user thread is done with it
};
ASSERT_SIZE(ThreadManager, 0x50);
} // namespace CTR
} // namespace snd
} // namespace nn
