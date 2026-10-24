#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"
#include "nn/snd/CTR/snd_Voice.h"
#include "nw/snd/snd_Global.h"
#include "nw/ut/internal/ut_LinkListImpl.h"

namespace nw {
namespace snd {
namespace internal {
struct WaveInfo;

namespace driver {
// A sound voice of one or two channels: up to two voices of nn::snd and their parameters, which
// Calc/Update send when they changed (the class and function names are from the reference symbols,
// where fefates calls the class MultiVoice; the members are ours).
class Voice
{
public:
    static const int CHANNEL_MAX = 2;
    // the priority that does not change the voices of nn::snd
    static const int PRIORITY_RELEASE = 1;

    // why the callback is called (names are ours)
    enum VoiceCallbackStatus
    {
        STATUS_FINISH_WAVE = 0, // the last wave buffer is done
        STATUS_CANCEL = 1,      // VoiceManager::Finalize
        STATUS_DROP_VOICE = 2,  // dropped for a voice of a higher priority
        STATUS_DROP_DSP = 3,    // nn::snd dropped a voice
    };
    typedef void (*Callback)(Voice* pVoice, VoiceCallbackStatus status, void* pArg); // (name is ours)

    // the parameters to send (m_SyncFlag; names are ours)
    enum UpdateFlag
    {
        UPDATE_START = 0x01,
        UPDATE_PAUSE = 0x02,
        UPDATE_PITCH = 0x04,
        UPDATE_MIX = 0x08,
        UPDATE_LPF = 0x10,
        UPDATE_BIQUAD = 0x20,
        UPDATE_VOLUME = 0x40,
    };

    // the constant pan positions of CalcMixParam (names are ours)
    static const f32 PAN_CENTER;
    static const f32 SURROUND_PAN_FRONT;
    static const f32 SURROUND_PAN_REAR;

    Voice(); // 0x004CB380 | nintendogs:bytes [tier B]
    ~Voice(); // 0x004CB3B0 | nintendogs:bytes [tier B]

    // the voices of nn::snd for channelCount channels (1 or 2)
    bool Alloc(int channelCount, int priority, Callback pCallback, void* pCallbackArg); // 0x004CAE34 | nintendogs:bytes [tier A]
    void Free(); // 0x004CAD70 | nintendogs:bytes [tier A]
    // the format of the wave
    void Initialize(const WaveInfo& waveInfo, u32 startOffset); // 0x004CA1E0 | nintendogs:callseq [tier A]
    void Start(); // 0x004CAFEC | nintendogs:bytes [tier A]
    void Stop(); // 0x004CADD8 | nintendogs:bytes [tier A]
    // after the last wave buffer: the callback
    void StopFinished(); // 0x004CA730 | nintendogs:callgraph [tier A]
    void Pause(bool isPause); // 0x004CAFCC | nintendogs:bytes [tier A]
    // sends the changed parameters (sound thread)
    void Calc(); // 0x004CAB24 | nintendogs:callgraph [tier A]
    // starts, pauses or resumes the voices of nn::snd
    void Update(); // 0x004CB030 | nintendogs:callgraph [tier A]

    void SetVolume(f32 volume); // 0x004CB348 | nintendogs:bytes [tier A]
    void SetPitch(f32 pitch); // 0x004CB2E8 | nintendogs:bytes [tier A]
    void SetPan(f32 pan); // 0x004CB00C | nintendogs:bytes [tier A]
    void SetSurroundPan(f32 surroundPan); // 0x004CA7EC | nintendogs:bytes [tier A]
    void SetPanMode(PanMode panMode); // 0x004CA2D8 | nintendogs:bytes [tier A]
    void SetPanCurve(PanCurve panCurve); // 0x004CA3A8 | nintendogs:bytes [tier A]
    void SetLpfFreq(f32 lpfFreq); // 0x004CA2B4 | nintendogs:bytes [tier A]
    void SetBiquadFilter(int type, f32 value); // 0x004CA810 | nintendogs:bytes [tier A]
    void SetMainSend(f32 send); // 0x004CA364 | nintendogs:bytes [tier A]
    void SetFxSend(AuxBus bus, f32 send); // 0x004CB30C | nintendogs:bytes [tier A]
    void SetPriority(int priority); // 0x004CA3C8 | nintendogs:bytes [tier A]
    void SetFrontBypass(bool isBypass); // 0x004CA7A8 | fefates:bytes [tier B] (MultiVoice)
    // (nintendogs calls this one SetFrontBypass; name after the call of nn::snd)
    void Set3dSurroundPreprocessed(bool isPreprocessed); // 0x004CA9AC (name is ours)
    // 0 4-point, 1 linear, 2 none
    void SetInterpolationType(u8 type); // 0x004CA900 | nintendogs:bytes [tier A]
    void SetAdpcmParam(int channelIndex, const nn::snd::CTR::AdpcmParam& param); // 0x004CA79C | nintendogs:callseq-callee [tier A]
    void AppendWaveBuffer(int channelIndex, nn::snd::CTR::WaveBuffer* pBuffer, bool isLast); // 0x004CA878 | nintendogs:bytes [tier A]
    void UpdateWaveBuffer(int channelIndex, nn::snd::CTR::WaveBuffer* pBuffer); // 0x004CA8A0 | fefates:bytes [tier B] (MultiVoice)
    // the priority again to the voices of nn::snd
    void UpdateVoicesPriority(); // 0x004CA964 | nintendogs:bytes [tier A]
    bool IsRun() const; // 0x00741424 | nintendogs:bytes [tier A]
    SampleFormat GetFormat() const; // 0x00741444 | nintendogs:bytes [tier A]

    // the mix of a channel to the speakers and the buses
    void CalcMixParam(int channelIndex, nn::snd::CTR::MixParam* pMixParam); // 0x004CA420 | nintendogs:callgraph [tier A]
    void CalcMix(); // 0x004CB220 | nintendogs:bytes [tier A]

    // the bytes of frames in the format
    static u32 FrameToByte(u32 frames, SampleFormat format); // 0x004CA2F8 | nintendogs:bytes [tier A]
    // the ADPCM state after offset frames of the data
    static void CalcOffsetAdpcmParam(nn::snd::CTR::AdpcmContext* pContext, const nn::snd::CTR::AdpcmParam& param, u32 offset,
                                     const void* pData); // 0x004CA8AC | nintendogs:bytes [tier A]
    static void SdkVoiceDropCallbackFunc(nn::snd::CTR::Voice* pVoice, uptr arg); // 0x004CA9EC | nintendogs:bytes [tier A]
    static void SdkVoiceDropCallbackFuncMulti(nn::snd::CTR::Voice* pVoice, uptr arg); // 0x004CAA3C | nintendogs:bytes [tier A]

    nn::snd::CTR::Voice* m_pVoices[CHANNEL_MAX];  // 0x00
    int m_VoiceCount;                             // 0x08
    Callback m_pCallback;                         // 0x0C
    void* m_pCallbackArg;                         // 0x10
    bool m_IsActive;                              // 0x14 allocated
    bool m_IsStart;                               // 0x15 Start was called
    bool m_IsStarted;                             // 0x16 the voices of nn::snd run
    bool m_IsPause;                               // 0x17
    bool m_IsPausing;                             // 0x18 the voices of nn::snd are paused
    u8 padding0x19;                               // 0x19
    bool m_IsAllocating;                          // 0x1A
    bool m_IsAllocFailed;                         // 0x1B a voice was dropped while allocating
    const nn::snd::CTR::WaveBuffer* m_pLastWaveBuffer; // 0x1C
    u16 m_SyncFlag;                               // 0x20 UpdateFlag
    u8 m_BiquadType;                              // 0x22
    bool m_IsFrontBypass;                         // 0x23
    f32 m_Volume;                                 // 0x24
    f32 m_Pitch;                                  // 0x28
    PanMode m_PanMode;                            // 0x2C
    PanCurve m_PanCurve;                          // 0x2D
    f32 m_Pan;                                    // 0x30
    f32 m_SurroundPan;                            // 0x34
    f32 m_LpfFreq;                                // 0x38
    f32 m_BiquadValue;                            // 0x3C
    int m_Priority;                               // 0x40
    f32 m_MainSend;                               // 0x44 1 + the send
    f32 m_FxSend[2];                              // 0x48
    SampleFormat m_Format;                        // 0x50
    u8 padding0x51[7];                            // 0x51
    ut::LinkListNode m_Link;                      // 0x58 (VoiceManager)
};
ASSERT_SIZE(Voice, 0x60);
} // namespace driver
} // namespace internal
} // namespace snd
} // namespace nw
