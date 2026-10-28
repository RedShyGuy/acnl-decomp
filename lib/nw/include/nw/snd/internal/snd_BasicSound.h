#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_MoveValue.h"
#include "nw/snd/snd_Global.h"
#include "nw/snd/snd_SoundActor.h"
#include "nw/ut/internal/ut_LinkListImpl.h"

namespace nw {
namespace snd {
class SoundHandle;
class SoundPlayer;

namespace internal {
class ExternalSoundPlayer;
namespace driver {
class BasicSoundPlayer;
} // namespace driver

// the type of a sound for the runtime type check: a pointer to the type of the base (name after nw::gfx)
struct TypeInfo
{
    // (inline)
    constexpr explicit TypeInfo(const TypeInfo* parent) : pParent(parent) {}

    const TypeInfo* pParent; // 0x0
};

// The base of the sounds in the game threads: the handles, the players it belongs to, the fades, pause and
// mute and the parameters it sends to its player in the sound thread (the class and function names are from
// the reference symbols, the members and the names of the unnamed functions are ours).
// RTTI N2nw3snd8internal10BasicSoundE @ 0x008D097C
// vtable 0x00903004 (vptr 0x0090300C), offset_to_top 0, 13 entries
class BasicSound
{
public:
    // the parameters a callback of the game sets for a sound (names are ours)
    struct AmbientParam
    {
        // (inline)
        AmbientParam()
            : volume(1.0f), pitch(1.0f), pan(0.0f), surroundPan(0.0f), fxSend(0.0f), lpfFreq(0.0f), biquadValue(0.0f), biquadType(0),
              priority(0), userData(0)
        {
        }

        f32 volume;       // 0x00
        f32 pitch;        // 0x04
        f32 pan;          // 0x08
        f32 surroundPan;  // 0x0C
        f32 fxSend;       // 0x10 to the first effect bus
        f32 lpfFreq;      // 0x14
        f32 biquadValue;  // 0x18
        int biquadType;   // 0x1C
        int priority;     // 0x20
        u32 userData;     // 0x24
    };
    ASSERT_SIZE(AmbientParam, 0x28);

    // the callbacks of the game for the ambient parameters (names are ours)
    class AmbientParamUpdateCallback
    {
    public:
        virtual ~AmbientParamUpdateCallback() {}
        virtual void UpdateAmbientParam(const void* pArg, u32 soundId, AmbientParam* pParam) = 0; // slot 0x08
        virtual int GetAmbientPriority(const void* pArg, u32 soundId) = 0; // slot 0x0C
    };
    class AmbientArgUpdateCallback
    {
    public:
        virtual ~AmbientArgUpdateCallback() {}
        virtual void UpdateAmbientArg(void* pArg, const BasicSound* pSound) = 0; // slot 0x08
    };
    class AmbientArgAllocatorCallback
    {
    public:
        virtual ~AmbientArgAllocatorCallback() {}
        virtual void* AllocAmbientArg(u32 argSize) = 0; // slot 0x08
        virtual void FreeAmbientArg(void* pArg, const BasicSound* pSound) = 0; // slot 0x0C
    };
    // the ambient callbacks and their argument (names are ours)
    struct AmbientInfo
    {
        AmbientParamUpdateCallback* paramUpdateCallback;   // 0x00
        AmbientArgUpdateCallback* argUpdateCallback;       // 0x04
        AmbientArgAllocatorCallback* argAllocatorCallback; // 0x08
        void* arg;                                         // 0x0C
        u32 argSize;                                       // 0x10
    };
    ASSERT_SIZE(AmbientInfo, 0x14);

    // (names are ours)
    enum State : u8
    {
        STATE_CONSTRUCTED = 0,
        STATE_INITIALIZED = 1,
        STATE_FINALIZED = 2,
        STATE_DESTRUCTED = 3,
    };
    enum PlayerState : u8
    {
        PLAYER_STATE_INIT = 0,
        PLAYER_STATE_PLAY = 1,
        PLAYER_STATE_STOP = 2,
    };
    enum PauseState : u8
    {
        PAUSE_STATE_NORMAL = 0,
        PAUSE_STATE_PAUSING = 1,
        PAUSE_STATE_PAUSED = 2,
        PAUSE_STATE_UNPAUSING = 3,
    };
    enum MuteState : u8
    {
        MUTE_STATE_NORMAL = 0,
        MUTE_STATE_MUTING = 1,
        MUTE_STATE_MUTED = 2,
        MUTE_STATE_UNMUTING = 3,
    };
    typedef void (*StopCallback)(); // (name is ours)

    static const TypeInfo s_TypeInfo;

    BasicSound(); // 0x004C57E4 (name is ours)
    virtual const TypeInfo* GetRuntimeTypeInfo() const; // 0x0073F2E8 slot 0x00 (name after nw::gfx)
    virtual ~BasicSound(); // 0x004C58C8 slot 0x04

    virtual void Initialize(); // 0x004C49AC slot 0x0C | fefates:bytes
    virtual void Finalize(); // 0x004C5604 slot 0x10 | fefates:bytes
    virtual bool IsPrepared() const = 0; // slot 0x14
    virtual bool IsAttachedTempSpecialHandle() = 0; // slot 0x18
    virtual void DetachTempSpecialHandle() = 0; // slot 0x1C
    virtual driver::BasicSoundPlayer* GetBasicSoundPlayerHandle() = 0; // slot 0x20
    virtual void OnUpdatePlayerPriority(); // 0x004C4F48 slot 0x24
    virtual void UpdateMoveValue(); // 0x004C4E3C slot 0x28 | fefates:bytes
    virtual void OnUpdateParam(); // 0x004C4D70 slot 0x2C
    virtual void UpdateParam(); // 0x004C4BBC slot 0x30 | fefates:bytes

    void StartPrepared(); // 0x004C4D74 | nintendogs:callgraph [tier A]
    void Stop(int fadeFrames); // 0x00137CC0 | fefates:bytes [tier B]
    void Pause(bool isPause, int fadeFrames); // 0x004C4FA4 | mk7dlp:callgraph [tier A]
    bool IsPause() const; // 0x0073F2F4 | fefates:bytes [tier B]
    void FadeIn(int frames); // 0x004C50AC | fefates:bytes [tier B]
    void Update(); // 0x004C511C | fefates:bytes [tier B]

    void SetAmbientInfo(const AmbientInfo& info); // 0x004C4D80 | nintendogs:bytes [tier A]
    static int GetAmbientPriority(const AmbientInfo& info, u32 soundId); // 0x004C4F08 | nintendogs:bytes [tier A]
    void SetPriority(int priority, int ambientPriority); // 0x004C4BB0 | fefates:callgraph [tier C]
    void SetPlayerPriority(int priority); // 0x004C4ED8 | fefates:bytes [tier B]
    void SetInitialVolume(f32 volume); // 0x004C4E7C | fefates:bytes [tier B]
    void SetVolume(f32 volume, int frames); // 0x004C5798 | fefates:bytes [tier B]
    void SetPitch(f32 pitch); // 0x004C5784 (name is ours)
    void SetPan(f32 pan); // 0x004C5114 (name is ours)
    void SetSurroundPan(f32 surroundPan); // 0x004C4E28 (name is ours)
    void SetBiquadFilter(int type, f32 value); // 0x004C4E30 (name is ours)
    void SetFxSend(AuxBus bus, f32 send); // 0x004C578C (name is ours)
    void SetAutoStopCounter(int frames); // 0x004C4F30 (name is ours)
    void SetPanMode(PanMode panMode); // 0x004C9150 | nintendogs:callseq [tier A]
    void SetPanCurve(PanCurve panCurve); // 0x004C4B54 | fefates:bytes [tier B]
    void SetFrontBypass(bool isFrontBypass); // 0x004C4DCC | fefates:bytes [tier B]

    bool IsAttachedGeneralHandle(); // 0x004C4F54 | nintendogs:callgraph [tier A]
    bool IsAttachedTempGeneralHandle(); // 0x004C4F94 | nintendogs:callgraph [tier A]
    void DetachGeneralHandle(); // 0x00137B28 | nintendogs:callgraph [tier A]
    void DetachTempGeneralHandle(); // 0x004C4F4C | nintendogs:callgraph [tier A]
    void DetachSoundPlayer(SoundPlayer* pPlayer); // 0x004C4ECC (name is ours)
    // (inline; name is ours) the priority with the ambient priority, 0 to 127
    int CalcCurrentPlayerPriority() const
    {
        const int priority = m_Priority + m_AmbientParam.priority;
        if (priority > 127) {
            return 127;
        }
        if (priority < 0) {
            return 0;
        }
        return priority;
    }
    void DetachExternalSoundPlayer(ExternalSoundPlayer* pPlayer); // 0x004C4F88 (name is ours)

    u32 m_Unknown0x04;                                 // 0x004
    SoundHandle* m_pGeneralHandle;                     // 0x008
    SoundHandle* m_pTempGeneralHandle;                 // 0x00C
    SoundPlayer* m_pSoundPlayer;                       // 0x010
    SoundActor* m_pSoundActor;                         // 0x014
    ExternalSoundPlayer* m_pExtSoundPlayer;            // 0x018
    AmbientInfo m_AmbientInfo;                         // 0x01C
    AmbientParam m_AmbientParam;                       // 0x030
    SoundActor::ActorParam m_ActorParam;               // 0x058
    MoveValue<f32, int> m_FadeVolume;                  // 0x068
    MoveValue<f32, int> m_PauseFadeVolume;             // 0x078
    MoveValue<f32, int> m_MuteFadeVolume;              // 0x088
    bool m_StartFlag;                                  // 0x098
    bool m_StartedFlag;                                // 0x099
    bool m_AutoStopFlag;                               // 0x09A
    bool m_FadeOutFlag;                                // 0x09B
    bool m_IsPlayerAvailable;                          // 0x09C set by the sound thread
    u8 m_Priority;                                     // 0x09D
    s8 m_BiquadType;                                   // 0x09E
    State m_State;                                     // 0x09F
    PlayerState m_PlayerState;                         // 0x0A0
    PauseState m_PauseState;                           // 0x0A1
    MuteState m_MuteState;                             // 0x0A2
    bool m_UnPauseFlag;                                // 0x0A3
    int m_AutoStopCounter;                             // 0x0A4
    u32 m_UpdateCounter;                               // 0x0A8 frames since the start
    u32 m_PlayingCounter;                              // 0x0AC frames not paused
    u32 m_Id;                                          // 0x0B0
    u32 m_InstanceId;                                  // 0x0B4
    MoveValue<f32, int> m_ExtMoveVolume;               // 0x0B8
    f32 m_InitVolume;                                  // 0x0C8
    f32 m_Pan;                                         // 0x0CC
    f32 m_Pitch;                                       // 0x0D0
    f32 m_LpfFreq;                                     // 0x0D4
    f32 m_BiquadValue;                                 // 0x0D8
    f32 m_MainSend;                                    // 0x0DC
    f32 m_FxSend[2];                                   // 0x0E0
    f32 m_SurroundPan;                                 // 0x0E8
    void* m_pUserParam;                                // 0x0EC
    u32 m_UserParamSize;                               // 0x0F0
    StopCallback m_StopCallback;                       // 0x0F4
    ut::LinkListNode m_PriorityLink;                   // 0x0F8 (SoundInstanceManager)
    ut::LinkListNode m_SoundPlayerPlayLink;            // 0x100 (SoundPlayer)
    ut::LinkListNode m_SoundPlayerPriorityLink;        // 0x108 (SoundPlayer)
    ut::LinkListNode m_ExtSoundPlayerPlayLink;         // 0x110 (ExternalSoundPlayer)
};
ASSERT_SIZE(BasicSound, 0x118);
// (inline) the sound as a T (NULL: not a T; name after nw::gfx)
template <typename T>
inline T* DynamicCast(BasicSound* pSound)
{
    for (const TypeInfo* pTypeInfo = pSound->GetRuntimeTypeInfo(); pTypeInfo != nullptr; pTypeInfo = pTypeInfo->pParent) {
        if (pTypeInfo == &T::s_TypeInfo) {
            return static_cast<T*>(pSound);
        }
    }
    return nullptr;
}
} // namespace internal
} // namespace snd
} // namespace nw
