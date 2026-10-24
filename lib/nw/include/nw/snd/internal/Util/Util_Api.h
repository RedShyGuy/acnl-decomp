#pragma once

#include "decomp.h"
#include "nw/snd/snd_Global.h"

namespace nw {
namespace snd {
class SoundArchive;

namespace internal {
class SoundArchiveLoader;

namespace Util {
// how a pan position becomes the volume of a side (the struct name is from the reference symbols,
// the members are ours)
struct PanInfo
{
    PanCurve curve;          // 0x0
    bool isCenterOne;        // 0x1 the volume is divided by the one of the center
    bool isClampedToOne;     // 0x2 at most 1 (else at most 2)
    bool isSurroundIgnored;  // 0x3 the stereo curves also in surround mode
};

// the next value of the random generator (a linear congruential generator; the top 16 bits)
u16 CalcRandom(); // 0x004C9BCC | nintendogs:bytes [tier A]
// the cutoff frequency in Hz of the low-pass filter for a value 0 (closed) to 1 (open)
u16 CalcLpfFreq(f32 scale); // 0x004C9BF8 | nintendogs:bytes [tier A]
// the volume of the right side for a pan position -1 (left) to 1 (right)
f32 CalcPanRatio(f32 pan, const PanInfo& panInfo); // 0x004C9C90 | fefates:bytes [tier B]
// the frequency ratio for a pitch in 1/256 semitones
f32 CalcPitchRatio(int pitch); // 0x004C9D98 | nintendogs:bytes [tier A]
// the amplitude ratio for a volume in dB (-90.4 to 6)
f32 CalcVolumeRatio(f32 dB); // 0x004C9EC4 | nintendogs:bytes [tier A]
// the bytes of samples samples in the format (0: an unknown format)
u32 GetByteBySample(u32 samples, SampleFormat format); // 0x004C9F1C | nintendogs:bytes [tier A]
// the volume of the rear for a surround pan position 0 (front) to 2 (rear)
f32 CalcSurroundPanRatio(f32 pan, const PanInfo& panInfo); // 0x004C9F8C | nintendogs:bytes [tier A]

// what GetWaveArchiveOfBank finds (names are ours)
enum WaveArchiveOfBankResult
{
    WAVE_ARCHIVE_OF_BANK_NO_TABLE = -2,     // the bank has no wave table
    WAVE_ARCHIVE_OF_BANK_NOT_LOADED = -1,   // the wave archive is not loaded
    WAVE_ARCHIVE_OF_BANK_LOADED = 0,        // loaded (and all waves loaded if loaded individually)
    WAVE_ARCHIVE_OF_BANK_NO_WAVES = 1,      // the bank uses no waves
    WAVE_ARCHIVE_OF_BANK_WAVE_MISSING = 2,  // loaded individually and a wave is missing
};
// the loaded wave archive of the first wave of a bank and whether it is loaded individually
// (symbols.json names it after a nintendogs IsLoadedWaveArchive with other parameters; the fefates
// bytes match this one)
int GetWaveArchiveOfBank(LoadItemInfo& loadItemInfo, bool& isLoadIndividual, const void* pBankFile, const SoundArchive& archive, const SoundArchiveLoader& loader); // 0x004CA014 | fefates:bytes [tier B]
// the wave of the first note of a wave sound if it is loaded (NULL: not)
const void* GetWaveFileOfWaveSound(const void* pWaveSoundFile, u32 index, const SoundArchive& archive, const SoundArchiveLoader& loader); // 0x004CA13C | fefates:bytes [tier B]
} // namespace Util
} // namespace internal
} // namespace snd
} // namespace nw
