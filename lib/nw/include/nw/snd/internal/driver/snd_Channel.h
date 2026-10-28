#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nw/snd/internal/driver/snd_DisposeCallback.h"
#include "nw/snd/internal/driver/snd_MultiVoice.h"
#include "nw/snd/internal/snd_CurveAdshr.h"
#include "nw/snd/internal/snd_CurveLfo.h"
#include "nw/snd/internal/snd_MoveValue.h"

namespace nw {
namespace snd {
namespace internal {
struct WaveInfo;

namespace driver {
// A note of a sequence or wave sound: a voice and its envelope, LFOs, sweep and parameters (the class
// and function names are from the reference symbols, the members are ours).
class Channel
{
public:
    class Disposer;

    static const int LFO_COUNT = 4;
    static const int WAVE_BUFFER_COUNT = 2; // the start and the loop
    // the interval of Update (ms)
    static const int UPDATE_INTERVAL_MSEC = 5;

    // why the callback is called (names are ours)
    enum ChannelCallbackStatus
    {
        CALLBACK_STATUS_STOPPED = 0,
        CALLBACK_STATUS_DROP = 1,
        CALLBACK_STATUS_FINISH = 2,
        CALLBACK_STATUS_CANCEL = 3,
    };
    typedef void (*ChannelCallback)(Channel* pChannel, ChannelCallbackStatus status, u32 userData); // (name is ours)

    // what an LFO changes (names are ours)
    enum LfoTarget : u8
    {
        LFO_TARGET_PITCH = 0,
        LFO_TARGET_VOLUME = 1,
        LFO_TARGET_PAN = 2,
    };

    // removes the channel when its wave data goes away
    class Disposer : public DisposeCallback
    {
    public:
        // (inline)
        explicit Disposer(Channel* pChannel) : m_pChannel(pChannel) {}
        virtual ~Disposer(); // 0x004D46C4 slot 0x00

        virtual void InvalidateData(const void* pStart, const void* pEnd); // 0x004D4558 slot 0x08 | nintendogs:callseq

        Channel* m_pChannel; // 0xC
    };

    Channel(); // 0x004D4800 | fefates:bytes [tier B]
    ~Channel(); // 0x004D489C | fefates:bytes [tier B]

    // a channel with a voice of voiceChannelCount channels (NULL: none free)
    static Channel* AllocChannel(int voiceChannelCount, int priority, ChannelCallback pCallback, u32 userData); // 0x004D3D04 | nintendogs:bytes-fuzzy [tier A]
    // the callback is not called any more
    static void FreeChannel(Channel* pChannel); // 0x004D3C70 | nintendogs:callgraph [tier A]
    static void VoiceCallbackFunc(MultiVoice* pVoice, MultiVoice::VoiceCallbackStatus status, void* pArg); // 0x004CCC2C | fefates:bytes [tier B]

    void InitParam(ChannelCallback pCallback, u32 userData); // 0x004D46C8 | fefates:bytes [tier B]
    // plays the wave from startOffset frames
    void Start(const WaveInfo& waveInfo, int length, u32 startOffset); // 0x004D4088 | fefates:bytes [tier B]
    void Stop(); // 0x004D4018 | nintendogs:callseq [tier A]
    // the envelope to its release (unless note off is ignored)
    void NoteOff(); // 0x004D44C4 | fefates:bytes [tier B]
    void Release(); // 0x004D4514 | fefates:bytes [tier B]
    // the parameters to the voice; isPeriodic: the curves move on
    void Update(bool isPeriodic); // 0x004D4118 | nintendogs:callseq [tier A]
    void AppendWaveBuffer(const WaveInfo& waveInfo, u32 startOffset); // 0x004D3DB0 | nintendogs:callseq [tier A]
    void SetLoopFlag(bool isLoop); // 0x004D3C88 | fefates:bytes [tier B]
    void SetSweepParam(f32 sweepPitch, int sweepTime, bool isAutoUpdate); // 0x004D3D8C | fefates:bytes [tier B]
    void UpdateSweep(int count); // 0x004D3CE4 | fefates:bytes [tier B]
    void SetBiquadFilter(int type, f32 value); // 0x004D3DA4 | fefates:bytes [tier B]
    // (inline; name is ours) pauses or resumes the voice of a playing channel
    void Pause(bool isPause)
    {
        if (m_ActiveFlag && m_PauseFlag != isPause) {
            m_PauseFlag = isPause;
            m_pVoice->Pause(isPause);
        }
    }

    nn::snd::CTR::WaveBuffer m_WaveBuffers[MultiVoice::CHANNEL_MAX][WAVE_BUFFER_COUNT]; // 0x000
    nn::snd::CTR::AdpcmContext m_AdpcmContexts[MultiVoice::CHANNEL_MAX];               // 0x060
    nn::snd::CTR::AdpcmContext m_AdpcmLoopContexts[MultiVoice::CHANNEL_MAX];           // 0x06C
    u32 m_StartOffset;                  // 0x078
    u32 m_LoopStartFrame;               // 0x07C
    u32 m_OriginalLoopStartFrame;       // 0x080
    bool m_IsLoop;                      // 0x084
    u8 padding0x85[3];                  // 0x085
    Disposer m_Disposer;                // 0x088
    CurveAdshr m_CurveAdshr;            // 0x098
    CurveLfo m_Lfos[LFO_COUNT];         // 0x0B4
    LfoTarget m_LfoTargets[LFO_COUNT];  // 0x134
    bool m_PauseFlag;                   // 0x138
    bool m_ActiveFlag;                  // 0x139
    bool m_AllocFlag;                   // 0x13A
    bool m_IsAutoSweep;                 // 0x13B
    bool m_IsReleasePriorityFix;        // 0x13C the priority stays at the release
    bool m_IsIgnoreNoteOff;             // 0x13D
    u8 m_BiquadType;                    // 0x13E
    u8 padding0x13F;                    // 0x13F
    f32 m_UserVolume;                   // 0x140
    f32 m_UserPitchRatio;               // 0x144
    f32 m_UserPan;                      // 0x148
    f32 m_UserSurroundPan;              // 0x14C
    f32 m_UserLpfFreq;                  // 0x150
    f32 m_BiquadValue;                  // 0x154
    f32 m_MainSend;                     // 0x158
    f32 m_FxSends[2];                   // 0x15C
    f32 m_UserPitch;                    // 0x164 in semitones
    f32 m_SweepPitch;                   // 0x168
    int m_SweepCounter;                 // 0x16C
    int m_SweepLength;                  // 0x170
    f32 m_InitPan;                      // 0x174
    f32 m_InitSurroundPan;              // 0x178
    f32 m_Tune;                         // 0x17C
    MoveValue<u8, u16> m_SilenceVolume; // 0x180
    f32 m_CachedCent;                   // 0x188
    f32 m_CachedPitchRatio;             // 0x18C of m_CachedCent
    int m_Length;                       // 0x190
    u8 m_Key;                           // 0x194
    u8 m_OriginalKey;                   // 0x195
    u8 m_KeyGroup;                      // 0x196 notes of a group release each other (name is ours)
    u8 m_InterpolationType;             // 0x197
    f32 m_InstrumentVolume;             // 0x198
    f32 m_Velocity;                     // 0x19C squared into the volume
    ChannelCallback m_pCallback;        // 0x1A0
    u32 m_CallbackUserData;             // 0x1A4
    MultiVoice* m_pVoice;               // 0x1A8
    Channel* m_pNextLink;               // 0x1AC the next channel of a sequence track (name is ours)
    ut::LinkListNode m_Link;            // 0x1B0 (ChannelManager)
};
ASSERT_SIZE(Channel, 0x1B8);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
