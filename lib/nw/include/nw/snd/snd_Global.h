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
    SAMPLE_FORMAT_DSP_ADPCM = 3, // 8-byte frames of 14 samples
};

// the speakers the sound is mixed for (only surround is known from the code)
enum OutputMode : u8
{
    OUTPUT_MODE_SURROUND = 2,
};

// how a pan position becomes the volumes of the two sides (after the shapes of the tables)
enum PanCurve : u8
{
    PAN_CURVE_SQRT = 0,
    PAN_CURVE_SINCOS = 1,
    PAN_CURVE_LINEAR = 2,
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

// an item of the sound archive and the address of its loaded file (the struct name is from the
// reference symbols, the members are ours)
struct LoadItemInfo
{
    u32 itemId;           // 0x0 (-1: none)
    const void* address;  // 0x4 (NULL: not loaded)
};
ASSERT_SIZE(LoadItemInfo, 0x8);
} // namespace internal

enum AuxBus : u8
{
};
enum PanMode : u8
{
};
enum SeqMute : u8
{
};
} // namespace snd
} // namespace nw
