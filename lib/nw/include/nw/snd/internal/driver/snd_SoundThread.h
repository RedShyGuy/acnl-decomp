#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/os/os_Tick.h"
#include "nw/ut/ut_LinkList.h"

namespace nw {
namespace snd {
// the time of a frame of the sound thread (the struct name is from the reference symbols, the member
// is ours)
struct SoundThreadProfile
{
    nn::os::Tick tick; // 0x0
};
ASSERT_SIZE(SoundThreadProfile, 0x8);

namespace internal {
namespace driver {
// The sound thread of nn::snd runs FrameProcess every sound frame: the callbacks, the commands of the
// game threads, the channels and the voices (the class and function names are from the reference
// symbols, the members are ours). It also measures its load.
class SoundThread
{
public:
    class PlayerCallback;
    class SoundFrameCallback;

    // the length of a sound frame (µs)
    static const int FRAME_LENGTH_USEC = 4889; // (name is ours)
    static const int LOAD_HISTOGRAM_SIZE = 101;

    // the load of the frames in percent of a frame (names are ours)
    struct LoadStat
    {
        f32 maxLoad;                            // 0x000
        f32 totalLoad;                          // 0x004
        u32 histogram[LOAD_HISTOGRAM_SIZE];     // 0x008 frames by load percent
        u32 frameCount;                         // 0x19C
        u32 overloadCount;                      // 0x1A0 below 0 or above 100 percent
    };
    ASSERT_SIZE(LoadStat, 0x1A4);
    // the load of a frame with the voice count (names are ours)
    struct LoadProfile
    {
        f32 load;        // 0x0
        u8 voiceCount;   // 0x4
        u8 unknown5;     // 0x5
        u8 padding[2];   // 0x6
    };
    ASSERT_SIZE(LoadProfile, 0x8);

    typedef ut::LinkList<SoundFrameCallback, 4> SoundFrameCallbackList;
    typedef ut::LinkList<PlayerCallback, 4> PlayerCallbackList;

    SoundThread(); // 0x004CBB44 | nintendogs:bytes [tier A]
    // (inline; registered by the guard of GetInstance)
    ~SoundThread() {} // 0x004CBBE4

    static SoundThread& GetInstance(); // 0x004CB3E4 | nintendogs:callgraph [tier A]

    void Initialize(); // 0x004CB3DC | nintendogs:bytes [tier A]
    // starts the thread of nn::snd (and the user thread on core 1); false on failure
    bool CreateSoundThread(uptr stackBuffer, u32 stackSize, int priority, uptr userStackBuffer, u32 userStackSize, int userPriority,
                           int coreNo, bool isEnableGetTick); // 0x004CB8A8 | nintendogs:callseq [tier A]
    void Destroy(); // 0x004CBAB0 | nintendogs:bytes [tier A]
    void Finalize(); // 0x004CBADC | nintendogs:bytes [tier A]

    void FrameProcess(); // 0x004CB4B8 | fefates:bytes [tier B]
    // the load statistics of a frame that took tick
    DECOMP_NOINLINE void CalcProcessCost(const nn::os::Tick& tick); // 0x004CB620 | nintendogs:callgraph [tier A]
    void SetProfileBuffer(SoundThreadProfile* pProfiles, int count); // 0x004CB854 | nintendogs:bytes [tier A]
    void ClearProfileBuffer(); // 0x004CB9E4 | nintendogs:bytes [tier A]

    void RegisterPlayerCallback(PlayerCallback* pCallback); // 0x004CBA38 | nintendogs:bytes [tier A]
    void UnregisterPlayerCallback(PlayerCallback* pCallback); // 0x00140F34 | nintendogs:callseq-callee [tier A]
    void RegisterSoundFrameCallback(SoundFrameCallback* pCallback); // 0x004CBA4C | fefates:bytes [tier B]
    void UnregisterSoundFrameCallback(SoundFrameCallback* pCallback); // 0x004CBA80 | fefates:bytes [tier B]

    // (inline; names are ours)
    void Lock() { m_CriticalSection.Enter(); }
    void Unlock() { m_CriticalSection.Exit(); }

    static void SoundThreadFunc(uptr arg); // 0x004CB43C | nintendogs:bytes [tier A]
    static void UserThreadCallback(uptr arg); // 0x004CBA18 | nintendogs:bytes [tier A]

    s64 m_ProcessTick;                          // 0x000 the ticks of all frames
    u32 m_ProcessCount;                         // 0x008
    u32 m_DspCycles;                            // 0x00C
    LoadStat m_LoadStat;                        // 0x010
    bool m_IsLoadStatEnabled;                   // 0x1B4
    u8 padding0x1B5[3];                         // 0x1B5
    SoundThreadProfile* m_pProfiles;            // 0x1B8
    int m_ProfileCount;                         // 0x1BC
    int m_ProfileIndex;                         // 0x1C0
    nn::os::CriticalSection m_CriticalSection;  // 0x1C4
    SoundFrameCallbackList m_SoundFrameCallbackList; // 0x1D0
    PlayerCallbackList m_PlayerCallbackList;    // 0x1DC
    void (*m_pUserCallback)(uptr arg);          // 0x1E8
    uptr m_UserCallbackArg;                     // 0x1EC
    int m_CoreNo;                               // 0x1F0
    bool m_IsCreated;                           // 0x1F4
    bool m_Unknown0x1F5;                        // 0x1F5
    bool m_IsEnableGetTick;                     // 0x1F6
    bool m_IsFinalizing;                        // 0x1F7
    LoadProfile* m_pLoadProfiles;               // 0x1F8
    u32 m_FrameUnknownCount;                    // 0x1FC stored with each load profile, 0 after each frame
    int m_LoadProfileCount;                     // 0x200
    int m_LoadProfileIndex;                     // 0x204
};
ASSERT_SIZE(SoundThread, 0x208);

// holds the lock of the sound thread while it lives (name is ours)
class SoundThreadLock
{
public:
    // (inline)
    SoundThreadLock() { SoundThread::GetInstance().Lock(); }
    ~SoundThreadLock() { SoundThread::GetInstance().Unlock(); }
};
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
