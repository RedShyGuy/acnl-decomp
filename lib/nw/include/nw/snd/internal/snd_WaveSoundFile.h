#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BinaryTypes.h"

namespace nw {
namespace snd {
namespace internal {
// The parts of a wave sound file (BCWSD; the class names are from the reference symbols, the members
// are ours): wave sounds, each with its parameters and notes that name a wave.
class WaveSoundFile
{
public:
    static const u32 SIGNATURE_FILE = 0x44535743; // 'CWSD'
    static const u32 VERSION_MIN = 0x01000000;
    static const u32 VERSION_MAX = 0x01000100;
    // from this version on the wave sound info has the filter values
    static const u32 VERSION_FILTER = 0x01000100;
    static const u16 TYPE_INFO_BLOCK = 0x6800;
    static const u16 TYPE_WAVE_SOUND_DATA = 0x4900;
    static const u16 TYPE_NOTE_INFO = 0x4902;

    class FileHeader;
    class InfoBlockBody;
    class WaveSoundData;
    class NoteInfo;
    class WaveSoundInfo;

    // the envelope of a wave sound or note without one
    static const AdshrCurve DEFAULT_ADSHR_CURVE; // (name is ours)
};

// the parameters of a wave sound (the struct name is from the reference symbols, the members are
// ours)
struct WaveSoundInfo
{
    f32 pitch;              // 0x00
    AdshrCurve adshrCurve;  // 0x04
    u8 pan;                 // 0x09
    s8 surroundPan;         // 0x0A
    u8 mainSend;            // 0x0B
    u8 fxSend[2];           // 0x0C
    u8 lpfFreq;             // 0x0E
    u8 biquadType;          // 0x0F
    u8 biquadValue;         // 0x10
};
ASSERT_SIZE(WaveSoundInfo, 0x14);

// a note of a wave sound (the struct name is from the reference symbols, the members are ours)
struct WaveSoundNoteInfo
{
    u32 waveArchiveId;      // 0x00
    u32 waveIndex;          // 0x04
    AdshrCurve adshrCurve;  // 0x08
    u8 originalKey;         // 0x0D
    u8 pan;                 // 0x0E
    u8 surroundPan;         // 0x0F
    u8 volume;              // 0x10
    f32 pitch;              // 0x14
};
ASSERT_SIZE(WaveSoundNoteInfo, 0x18);
} // namespace internal
} // namespace snd
} // namespace nw
