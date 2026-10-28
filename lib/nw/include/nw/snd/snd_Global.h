#pragma once

#include "decomp.h"

namespace nw {
namespace snd {
// The enums of the sound library (the type names are from the reference symbols; the values that
// are known and their names are ours).

// the format of the samples of a wave
enum SampleFormat : u8
{
    SAMPLE_FORMAT_PCM_S8 = 0,
    SAMPLE_FORMAT_PCM_S16 = 1,
    SAMPLE_FORMAT_PCM_S32 = 2,   // the aux buffers of the effects
    SAMPLE_FORMAT_DSP_ADPCM = 3, // 8-byte frames of 14 samples
};

// the speakers the sound is mixed for (as nn::snd::CTR::OutputMode)
enum OutputMode : u8
{
    OUTPUT_MODE_MONO = 0,
    OUTPUT_MODE_STEREO = 1,
    OUTPUT_MODE_SURROUND = 2,
};

// how a pan position becomes the volumes of the two sides: the shape, whether the center is at full
// volume (0 dB) and whether the volume is at most 1 (from Voice::CalcMixParam; names are ours)
enum PanCurve : u8
{
    PAN_CURVE_SQRT = 0,
    PAN_CURVE_SQRT_0DB = 1,
    PAN_CURVE_SQRT_0DB_CLAMP = 2,
    PAN_CURVE_SINCOS = 3,
    PAN_CURVE_SINCOS_0DB = 4,
    PAN_CURVE_SINCOS_0DB_CLAMP = 5,
    PAN_CURVE_LINEAR = 6,
    PAN_CURVE_LINEAR_0DB = 7,
    PAN_CURVE_LINEAR_0DB_CLAMP = 8,
};

// the decoder state of DSP ADPCM (3dbrew "BCWAV": the DSP ADPCM info; names are ours)
struct DspAdpcmParam
{
    u16 coef[16];       // 0x00
    u16 predScale;      // 0x20
    u16 yn1;            // 0x22
    u16 yn2;            // 0x24
};
ASSERT_SIZE(DspAdpcmParam, 0x26);

namespace internal {
// the decoder state at the loop start
struct DspAdpcmLoopParam
{
    u16 loopPredScale; // 0x0
    u16 loopYn1;       // 0x2
    u16 loopYn2;       // 0x4
};
ASSERT_SIZE(DspAdpcmLoopParam, 0x6);

// what the game can read of a playing stream (the struct name is from the reference symbols, the
// members are ours)
struct StreamDataInfo
{
    bool isLoop;        // 0x0
    u32 sampleRate;     // 0x4
    u32 loopStart;      // 0x8
    u32 loopEnd;        // 0xC
};
ASSERT_SIZE(StreamDataInfo, 0x10);

// an item of the sound archive and the address of its loaded file (the struct name is from the
// reference symbols, the members are ours)
struct LoadItemInfo
{
    // (inline; ARMCC's copy for array constructions)
    DECOMP_EMIT_COPY LoadItemInfo() : itemId(0xFFFFFFFF), address(nullptr) {} // 0x004C72AC (name is ours)

    u32 itemId;           // 0x0 (-1: none)
    const void* address;  // 0x4 (NULL: not loaded)
};
ASSERT_SIZE(LoadItemInfo, 0x8);
} // namespace internal

// the effect buses (as nn::snd::CTR::AuxBusId)
enum AuxBus : u8
{
    AUX_BUS_A = 0,
    AUX_BUS_B = 1,
};
// how the pan of a sound with two channels works (names are ours)
enum PanMode : u8
{
    PAN_MODE_DUAL = 0,    // each channel panned around its side
    PAN_MODE_BALANCE = 1, // the pan is the balance of the two channels
};
// the mute of sequence tracks (names are ours, values from SequenceTrack::SetMute)
enum SeqMute : u8
{
    SEQ_MUTE_OFF = 0,       // playing
    SEQ_MUTE_NO_STOP = 1,   // no new notes, the playing ones go on
    SEQ_MUTE_RELEASE = 2,   // the playing notes are released
    SEQ_MUTE_STOP = 3,      // the playing notes stop at once
};
} // namespace snd
} // namespace nw
