#pragma once

#include "decomp.h"
#include "nn/os/os_CriticalSection.h"
#include "nn/snd/CTR/snd_Voice.h"

namespace nn {
namespace snd {
namespace CTR {
// the status of a channel that the DSP writes each frame (names are ours)
struct DspChannelStatus
{
    u16 m_Flags;            // 0x0, low byte: playing; high byte: the buffer changed
    s16 m_SyncCount;        // 0x2
    u32 m_Position;         // 0x4, halves swapped
    u16 m_CurrentBufferId;  // 0x8
    u16 m_LastBufferId;     // 0xA
};
ASSERT_SIZE(DspChannelStatus, 0xC);

// The DSP side of a voice: sends the settings that changed (m_DirtyFlags) and the queue of wave
// buffers to the DSP channel. Member names are ours.
class VoiceImpl
{
public:
    static const u16 DIRTY_MIX = 0x1;
    static const u16 DIRTY_RATE = 0x2;
    static const u16 DIRTY_FILTER_TYPE = 0x4;
    static const u16 DIRTY_MONO_FILTER = 0x8;
    static const u16 DIRTY_BIQUAD_FILTER = 0x10;
    static const u16 DIRTY_INTERPOLATION = 0x20;
    static const u16 DIRTY_ADPCM_PARAM = 0x40;
    static const u16 DIRTY_SYNC_COUNT = 0x8000;

    // the wave buffers the DSP queues (one playing and the next ones)
    static const s32 SENT_BUFFER_MAX = 5;
    static const s32 NEXT_BUFFER_SLOTS = 4;

    explicit VoiceImpl(int id); // 0x00467064 | nintendogs:bytes [confirmed by fefates] [tier A]
    void Initialize(); // 0x00466374 | nintendogs:callgraph [confirmed by fefates] [tier A]
    void SetMixParam(const MixParam& mixParam); // 0x004664A8 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetMixVolume(); // 0x004664E8 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetSyncCount(); // 0x004665D0 | nintendogs:bytes [confirmed by fefates] [tier A]
    void ForceUpdateDspParams(); // 0x0046660C (name is ours)
    void UpdateDspParams(); // 0x00466618 (name is ours)
    void UpdateStatus(const void* status); // 0x004668E8 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetAdpcmParam(const AdpcmParam& param); // 0x00466A3C (name is ours)
    void SetFilterType(FilterType type); // 0x00466A80 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetSampleRate(int sampleRate); // 0x00466AAC | nintendogs:bytes [confirmed by fefates] [tier A]
    void SendWaveBuffer(); // 0x00466ACC | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void SetChannelCount(int count); // 0x00466CD4 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void SetSampleFormat(SampleFormat format); // 0x00466CF4 | nintendogs:bytes [confirmed by fefates] [tier A]
    void AppendWaveBuffer(WaveBuffer* buffer); // 0x00466D18 | fefates:bytes [tier B]
    void UpdateWaveBuffer(WaveBuffer* buffer); // 0x00466DBC | fefates:bytes [tier B]
    void ReleaseWaveBuffer(); // 0x00466E1C | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetFrontBypassFlag(bool isBypass); // 0x00466E90 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void SetInterpolationType(InterpolationType type); // 0x00466EB0 | nintendogs:bytes [confirmed by fefates] [tier A]
    void UpdateWaveBufferList(); // 0x00466EC4 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void Set3dSurroundPreprocessed(bool isPreprocessed); // 0x00466EE0 | fefates:bytes [tier B]
    void SetMonoFilterCoefficients(const MonoFilterCoefficients& coefficients); // 0x00466EF4 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetBiquadFilterCoefficients(const BiquadFilterCoefficients& coefficients); // 0x00466F34 | nintendogs:bytes [confirmed by fefates] [tier A]
    void Start(); // 0x00466F5C | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetPitch(float pitch); // 0x00466F84 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetState(Voice::State state); // 0x00466FD8 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetVolume(float volume); // 0x00467050 | nintendogs:bytes [confirmed by fefates] [tier A]

    s32 m_Id;                                   // 0x00
    s16 m_SyncCount;                            // 0x04, to match the status of the DSP
    u16 m_NextBufferId;                         // 0x06
    u32 m_PlayPosition;                         // 0x08
    bool m_IsPlaying;                           // 0x0C
    u8 m_State;                                 // 0x0D, Voice::State
    u8 m_InterpolationType;                     // 0x0E
    u8 m_FilterType;                            // 0x0F
    MonoFilterCoefficients m_MonoFilter;        // 0x10
    BiquadFilterCoefficients m_BiquadFilter;    // 0x14
    DspsndAudioInfo m_AudioInfo;                // 0x1E
    s32 m_SampleRate;                           // 0x20
    f32 m_Pitch;                                // 0x24
    f32 m_TimerRate;                            // 0x28, samples per output sample
    u32 m_DspCycles;                            // 0x2C, estimated
    WaveBuffer* m_WaveBuffers;                  // 0x30
    s16 m_SentBufferCount;                      // 0x34
    s16 m_NextSlot;                             // 0x36
    MixParam m_MixParam;                        // 0x38
    f32 m_Volume;                               // 0x68
    u16 m_DirtyFlags;                           // 0x6C
    bool m_HasAdpcmContext;                     // 0x6E
    u8 m_BufferListFlags;                       // 0x6F, the list has to be sent again
    nn::os::CriticalSection m_Lock;             // 0x70
    AdpcmParam m_AdpcmParam;                    // 0x7C
};
ASSERT_SIZE(VoiceImpl, 0x9C);
} // namespace CTR
} // namespace snd
} // namespace nn
