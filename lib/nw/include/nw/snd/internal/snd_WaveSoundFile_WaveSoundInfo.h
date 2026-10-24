#pragma once

#include "decomp.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

// the optional parameters of a wave sound
class nw::snd::internal::WaveSoundFile::WaveSoundInfo
{
public:
    // the bits of the optional values (names are ours)
    enum ParameterBit
    {
        PARAMETER_PAN = 0,    // byte 0 pan, byte 1 surround pan
        PARAMETER_PITCH = 1,
        PARAMETER_FILTER = 2, // byte 0 low-pass frequency, byte 1 biquad type, byte 2 biquad value
        PARAMETER_SEND = 8,   // the offset of the sends: main, count, fx sends
        PARAMETER_ADSHR_CURVE = 9,
    };
    // the fx sends that are read at most
    static const u8 FX_SEND_COUNT_MAX = 2; // (name is ours)

    u8 GetLpfFreq() const; // 0x0073F48C | fefates:bytes [tier B]
    // the main send and fxSendCount fx sends (127 and 0 if not set)
    void GetSendValue(u8* pMainSend, u8* pFxSend, u8 fxSendCount) const; // 0x0073F4B0 | nintendogs:bytes [tier A]
    const AdshrCurve* GetAdshrCurve() const; // 0x0073F59C | nintendogs:bytes [tier A]
    u8 GetBiquadType() const; // 0x0073F5D4 | fefates:bytes [tier B]
    u8 GetBiquadValue() const; // 0x0073F5FC | fefates:bytes [tier B]
    s8 GetSurroundPan() const; // 0x0073F624 | nintendogs:bytes [tier A]
    u8 GetPan() const; // 0x0073F64C | nintendogs:bytes [tier A]
    f32 GetPitch() const; // 0x0073F670 | nintendogs:bytes [tier A]

    Util::BitFlag optionParams; // 0x0
};
