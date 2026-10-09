#pragma once

#include "decomp.h"
#include "nn/snd/CTR/snd_Types.h"

namespace nn {
namespace snd {
namespace CTR {
class VoiceImpl;

// A voice: one channel of the DSP. The settings are kept here and passed on to the VoiceImpl,
// which sends them to the DSP. Member names are ours.
class Voice
{
public:
    enum State : u8
    {
        STATE_PLAY = 0,
        STATE_STOP = 1,
        STATE_PAUSE = 2
    };

    static const s32 PRIORITY_MAX = 0x7FFF;
    static const s32 DEFAULT_SAMPLE_RATE = 32728;

    explicit Voice(int id); // 0x00463810 | nintendogs:bytes [confirmed by fefates] [tier A]
    void Initialize(); // 0x00463634 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetPriority(int priority); // 0x00463710 | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void EnableBiquadFilter(bool isEnabled); // 0x00463744 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetMonoFilterCoefficients(u16 cutoffFrequency); // 0x00463764 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetPitch(float pitch); // 0x004637F0 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetMixParam(const MixParam& mixParam); // 0x0046647C | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void EnableMonoFilter(bool isEnabled); // 0x00466A60 | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetSampleRate(int sampleRate); // 0x00466A94 | nintendogs:bytes [confirmed by fefates] [tier A]
    void AppendWaveBuffer(WaveBuffer* buffer); // 0x00466D10 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
    void SetFrontBypassFlag(bool isBypass); // 0x00466E88 | fefates:callgraph [tier C]
    void SetInterpolationType(InterpolationType type); // 0x00466EA4 | nintendogs:callgraph [confirmed by fefates, mk7dlp] [tier A]
    void Set3dSurroundPreprocessed(bool isPreprocessed); // 0x00466ED8 | fefates:callgraph [tier C]
    void SetBiquadFilterCoefficients(const BiquadFilterCoefficients& coefficients); // 0x00466F0C | nintendogs:bytes [confirmed by fefates] [tier A]
    void SetState(State state); // 0x00466FAC | nintendogs:bytes [confirmed by fefates, mk7dlp] [tier A]
    void SetVolume(float volume); // 0x00467044 | fefates:callgraph [tier C]
    // (names are ours)
    void SetChannelCount(int count); // 0x00466CCC (name is ours)
    void SetSampleFormat(SampleFormat format); // 0x00466CEC (name is ours)
    void UpdateWaveBuffer(WaveBuffer* buffer); // 0x00466DB4 (name is ours)
    void SetAdpcmParam(const AdpcmParam& param); // 0x00466A34 (name is ours)

    s32 m_Id;                                   // 0x00
    u8 m_State;                                 // 0x04
    u8 m_InterpolationType;                     // 0x05
    u8 m_Padding6;                              // 0x06
    u8 m_FilterType;                            // 0x07, FilterType bits
    MonoFilterCoefficients m_MonoFilter;        // 0x08
    BiquadFilterCoefficients m_BiquadFilter;    // 0x0C
    u8 m_Padding16[2];                          // 0x16
    s32 m_SampleRate;                           // 0x18
    f32 m_Pitch;                                // 0x1C
    s32 m_Priority;                             // 0x20
    Voice* m_Prev;                              // 0x24, the list of VoiceManager (by priority)
    Voice* m_Next;                              // 0x28
    VoiceDropCallback m_DropCallback;           // 0x2C
    uptr m_DropCallbackArg;                     // 0x30
    MixParam m_MixParam;                        // 0x34
    f32 m_Volume;                               // 0x64
    VoiceImpl* m_Impl;                          // 0x68
};
ASSERT_SIZE(Voice, 0x6C);
} // namespace CTR
} // namespace snd
} // namespace nn
