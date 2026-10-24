#include "nw/snd/internal/snd_WaveSoundFile_WaveSoundInfo.h"

namespace nw {
namespace snd {
namespace internal {
// 0x0073F48C | fefates:bytes [tier B]
u8 nw::snd::internal::WaveSoundFile::WaveSoundInfo::GetLpfFreq() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_FILTER)) {
        return 64;
    }
    return value & 0xFF;
}

// 0x0073F4B0 | nintendogs:bytes [tier A]
void nw::snd::internal::WaveSoundFile::WaveSoundInfo::GetSendValue(u8* pMainSend, u8* pFxSend, u8 fxSendCount) const
{
    u32 offset;
    if (!optionParams.GetValue(&offset, PARAMETER_SEND)) {
        *pMainSend = 127;
        for (int i = 0; i < fxSendCount; ++i) {
            pFxSend[i] = 0;
        }
        return;
    }
    const u8* pSend = reinterpret_cast<const u8*>(this) + offset;
    *pMainSend = pSend[0];
    const int count = pSend[1] > FX_SEND_COUNT_MAX ? FX_SEND_COUNT_MAX : pSend[1];
    for (int i = 0; i < count; ++i) {
        pFxSend[i] = pSend[2 + i];
    }
}

// 0x0073F59C | nintendogs:bytes [tier A]
const nw::snd::internal::AdshrCurve* nw::snd::internal::WaveSoundFile::WaveSoundInfo::GetAdshrCurve() const
{
    u32 offset;
    if (!optionParams.GetValue(&offset, PARAMETER_ADSHR_CURVE)) {
        return &DEFAULT_ADSHR_CURVE;
    }
    const BinaryReference* pReference = reinterpret_cast<const BinaryReference*>(reinterpret_cast<const u8*>(this) + offset);
    return static_cast<const AdshrCurve*>(GetReferedPointer(pReference, *pReference));
}

// 0x0073F5D4 | fefates:bytes [tier B]
u8 nw::snd::internal::WaveSoundFile::WaveSoundInfo::GetBiquadType() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_FILTER)) {
        return 0;
    }
    return (value >> 8) & 0xFF;
}

// 0x0073F5FC | fefates:bytes [tier B]
u8 nw::snd::internal::WaveSoundFile::WaveSoundInfo::GetBiquadValue() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_FILTER)) {
        return 0;
    }
    return (value >> 16) & 0xFF;
}

// 0x0073F624 | nintendogs:bytes [tier A]
s8 nw::snd::internal::WaveSoundFile::WaveSoundInfo::GetSurroundPan() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_PAN)) {
        return 0;
    }
    return static_cast<s8>(value >> 8);
}

// 0x0073F64C | nintendogs:bytes [tier A]
u8 nw::snd::internal::WaveSoundFile::WaveSoundInfo::GetPan() const
{
    u32 value;
    if (!optionParams.GetValue(&value, PARAMETER_PAN)) {
        return 64;
    }
    return value & 0xFF;
}

// 0x0073F670 | nintendogs:bytes [tier A]
f32 nw::snd::internal::WaveSoundFile::WaveSoundInfo::GetPitch() const
{
    f32 value;
    if (!optionParams.GetValueF32(&value, PARAMETER_PITCH)) {
        return 1.0f;
    }
    return value;
}

} // namespace internal
} // namespace snd
} // namespace nw
