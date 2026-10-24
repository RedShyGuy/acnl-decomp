#include "nw/snd/internal/snd_BankFile_VelocityRegion.h"

namespace nw {
namespace snd {
namespace internal {
namespace {
// the envelope of a velocity region without one
// 0x00982894
const AdshrCurve s_DefaultAdshrCurve(127, 127, 127, 127, 127);

// sets s_DefaultAdshrCurve
// 0x00796820 (static initializer)
} // namespace

// 0x00742D88 | nintendogs:bytes [tier A]
u8 nw::snd::internal::BankFile::VelocityRegion::GetKeyGroup() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_NOTE)) {
        return 0;
    }
    return (value >> 8) & 0xFF;
}

// 0x00742DB4 | nintendogs:bytes [tier A]
const nw::snd::internal::AdshrCurve* nw::snd::internal::BankFile::VelocityRegion::GetAdshrCurve() const
{
    u32 offset;
    if (!optionParams.GetValue(&offset, PARAMETER_ADSHR_CURVE)) {
        return &s_DefaultAdshrCurve;
    }
    const BinaryReference* pReference = reinterpret_cast<const BinaryReference*>(reinterpret_cast<const u8*>(this) + offset);
    return static_cast<const AdshrCurve*>(GetReferedPointer(pReference, *pReference));
}

// 0x00742DF0 | nintendogs:bytes [tier A]
u8 nw::snd::internal::BankFile::VelocityRegion::GetOriginalKey() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_ORIGINAL_KEY)) {
        return 60;
    }
    return value & 0xFF;
}

// 0x00742E18 | nintendogs:bytes [tier A]
bool nw::snd::internal::BankFile::VelocityRegion::IsIgnoreNoteOff() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_NOTE)) {
        return false;
    }
    return (value & 0xFF) != 0;
}

// 0x00742E44 | nintendogs:bytes [tier A]
const nw::snd::internal::BankFile::VelocityRegion::RegionParameter* nw::snd::internal::BankFile::VelocityRegion::GetRegionParameter() const
{
    if (optionParams.m_Flags != PARAMETER_FLAGS_ALL) {
        return nullptr;
    }
    return reinterpret_cast<const RegionParameter*>(&optionParams.m_Flags + 1);
}

// 0x00742E5C | nintendogs:bytes [tier A]
u8 nw::snd::internal::BankFile::VelocityRegion::GetInterpolationType() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_NOTE)) {
        return 0;
    }
    return (value >> 16) & 0xFF;
}

// 0x00742E88 | nintendogs:bytes [tier A]
u8 nw::snd::internal::BankFile::VelocityRegion::GetPan() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_PAN)) {
        return 64;
    }
    return value & 0xFF;
}

// 0x00742EB0 | nintendogs:bytes [tier A]
f32 nw::snd::internal::BankFile::VelocityRegion::GetPitch() const
{
    f32 value;
    if (!optionParams.GetValueF32(&value, PARAMETER_PITCH)) {
        return 1.0f;
    }
    return value;
}

// 0x00742F24 | nintendogs:bytes [tier A]
u8 nw::snd::internal::BankFile::VelocityRegion::GetVolume() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_VOLUME)) {
        return 127;
    }
    return value & 0xFF;
}

} // namespace internal
} // namespace snd
} // namespace nw
