#pragma once

#include "decomp.h"

// The types of nn::snd. The type names are from the symbols; all member and enumerator names are
// ours.
namespace nn {
namespace snd {
namespace CTR {
class Voice;

enum AuxBusId : s8
{
    AUX_BUS_NONE = -1,
    AUX_BUS_0 = 0,
    AUX_BUS_1 = 1,
    AUX_BUS_COUNT = 2
};

enum SyncMode : u8
{
};

enum ClippingMode : u8
{
};

enum OutputMode : u8
{
    OUTPUT_MODE_MONO = 0,
    OUTPUT_MODE_STEREO = 1,
    OUTPUT_MODE_SURROUND = 2
};

enum SurroundSpeakerPosition : u8
{
};

enum FilterType : u8
{
    FILTER_TYPE_MONO = 1,     // bit 0
    FILTER_TYPE_BIQUAD = 2    // bit 1
};

enum InterpolationType : u8
{
};

enum SampleFormat : u8
{
    SAMPLE_FORMAT_PCM8 = 0,
    SAMPLE_FORMAT_PCM16 = 1,
    SAMPLE_FORMAT_ADPCM = 2
};

// the coefficients of the DSP-ADPCM decoder (8 pairs)
struct AdpcmParam
{
    s16 m_Coefficients[16];
};
ASSERT_SIZE(AdpcmParam, 0x20);

struct AdpcmContext
{
    u16 m_PredScale;    // the last frame header
    s16 m_History[2];
};
ASSERT_SIZE(AdpcmContext, 0x6);

// a buffer of samples, queued in a voice
struct WaveBuffer
{
    enum Status : u8
    {
        STATUS_FREE = 0,
        STATUS_WAIT = 1,
        STATUS_PLAY = 2,
        STATUS_DONE = 3,
        STATUS_TO_BE_DELETED = 4
    };

    const void* m_BufferAddress;    // 0x00
    u32 m_SampleLength;             // 0x04
    AdpcmContext* m_AdpcmContext;   // 0x08, for ADPCM: the state at the start
    u32 m_UserParam;                // 0x0C
    bool m_IsLoop;                  // 0x10
    u8 m_Status;                    // 0x11
    u16 m_BufferId;                 // 0x12
    WaveBuffer* m_Next;             // 0x14
};
ASSERT_SIZE(WaveBuffer, 0x18);

// the volumes of a voice for the main bus and the two aux buses (front left/right, rear
// left/right)
struct MixParam
{
    MixParam()
    {
        for (s32 i = 0; i < 4; i++) {
            m_Aux[1][i] = 0.0f;
            m_Aux[0][i] = 0.0f;
            m_Main[i] = 0.0f;
        }
    }

    f32 m_Main[4];
    f32 m_Aux[AUX_BUS_COUNT][4];
};
ASSERT_SIZE(MixParam, 0x30);

// the samples of an aux bus (one frame of 160 samples per channel)
struct AuxBusData
{
    s32* m_FrontLeft;
    s32* m_FrontRight;
    s32* m_RearLeft;
    s32* m_RearRight;
};
ASSERT_SIZE(AuxBusData, 0x10);

struct MonoFilterCoefficients
{
    s16 m_A0;
    s16 m_B0;
};
ASSERT_SIZE(MonoFilterCoefficients, 0x4);

struct BiquadFilterCoefficients
{
    s16 m_Values[5];
};
ASSERT_SIZE(BiquadFilterCoefficients, 0xA);

// the lengths of the filters of a reverb (in samples)
struct ReverbFilterSize
{
    u32 m_Comb[2];
    u32 m_AllPass;
};
ASSERT_SIZE(ReverbFilterSize, 0xC);

// the stack and the priority of the sound thread
struct ThreadParameter
{
    uptr m_StackBuffer;
    u32 m_StackSize;
    s32 m_Priority;
};
ASSERT_SIZE(ThreadParameter, 0xC);

// the parameters of the delay effect of the DSP; m_Flags says which parts are set
struct DspFxDelayParams
{
    static const u16 FLAG_ENABLE = 1;
    static const u16 FLAG_BUFFER = 2;
    static const u16 FLAG_PARAM = 4;

    u16 m_Flags;            // 0x00
    u16 m_IsEnabled;        // 0x02
    u16 m_Padding;          // 0x04
    u16 m_ChannelCount;     // 0x06
    u32 m_BufferAddress;    // 0x08, the device address with its halves swapped
    u16 m_DelaySamples;     // 0x0C
    u16 m_Feedback;         // 0x0E, 1.7 fixed point
    u16 m_LpfB;             // 0x10
    u16 m_LpfA;             // 0x12
};
ASSERT_SIZE(DspFxDelayParams, 0x14);

struct DspFxReverbParams
{
    u16 m_Flags;                // 0x00
    u16 m_IsEnabled;            // 0x02
    u16 m_Padding;              // 0x04
    u16 m_ChannelCount;         // 0x06
    u32 m_BufferAddress[5];     // 0x08
    u16 m_EarlyReflectionSamples;   // 0x1C
    u16 m_FusedSamples;         // 0x1E
    u16 m_FilterSize[3];        // 0x20
    u16 m_EarlyGain;            // 0x26
    u16 m_FusedGain;            // 0x28
    u16 m_Coloration;           // 0x2A
    u16 m_CombGain[2];          // 0x2C
    u16 m_LpfB;                 // 0x30
    u16 m_LpfA;                 // 0x32
};
ASSERT_SIZE(DspFxReverbParams, 0x34);

// the format of the samples of a voice
struct DspsndAudioInfo
{
    u16 m_ChannelCount : 2;
    u16 m_SampleFormat : 2;
    u16 m_IsFrontBypass : 1;
    u16 m_Interpolation : 1;
    u16 m_Is3dSurroundPreprocessed : 1;
    u16 m_Padding : 9;
};
ASSERT_SIZE(DspsndAudioInfo, 0x2);

typedef void (*VoiceDropCallback)(Voice* voice, uptr arg);
typedef void (*AuxCallback)(AuxBusData* data, s32 sampleCount, uptr arg);
} // namespace CTR
} // namespace snd
} // namespace nn
