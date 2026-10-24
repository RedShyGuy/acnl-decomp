#include "nw/snd/internal/snd_WaveSoundFile_NoteInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0013D238 | nintendogs:callgraph [tier A]
const nw::snd::internal::AdshrCurve* nw::snd::internal::WaveSoundFile::NoteInfo::GetAdshrCurve() const
{
    u32 offset;
    if (!optionParams.GetValue(&offset, PARAMETER_ADSHR_CURVE)) {
        return &DEFAULT_ADSHR_CURVE;
    }
    const BinaryReference* pReference = reinterpret_cast<const BinaryReference*>(reinterpret_cast<const u8*>(this) + offset);
    return static_cast<const AdshrCurve*>(GetReferedPointer(pReference, *pReference));
}

// 0x0013D274 | nintendogs:bytes [tier A]
u8 nw::snd::internal::WaveSoundFile::NoteInfo::GetOriginalKey() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_ORIGINAL_KEY)) {
        return 64;
    }
    return value & 0xFF;
}

// 0x0013D29C | nintendogs:bytes [tier A]
u8 nw::snd::internal::WaveSoundFile::NoteInfo::GetSurroundPan() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_PAN)) {
        return 0;
    }
    return (value >> 8) & 0xFF;
}

// 0x0013D2C8 | nintendogs:callgraph [tier A]
u8 nw::snd::internal::WaveSoundFile::NoteInfo::GetPan() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_PAN)) {
        return 64;
    }
    return value & 0xFF;
}

// 0x0013D2F0 | nintendogs:callgraph [tier A]
f32 nw::snd::internal::WaveSoundFile::NoteInfo::GetPitch() const
{
    f32 value;
    if (!optionParams.GetValueF32(&value, PARAMETER_PITCH)) {
        return 1.0f;
    }
    return value;
}

// 0x0013D364 | nintendogs:bytes [tier A]
u8 nw::snd::internal::WaveSoundFile::NoteInfo::GetVolume() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_VOLUME)) {
        return 96;
    }
    return value & 0xFF;
}

} // namespace internal
} // namespace snd
} // namespace nw
