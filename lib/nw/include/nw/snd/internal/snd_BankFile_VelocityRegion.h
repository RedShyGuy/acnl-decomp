#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

// a sound of an instrument: a wave and its optional parameters
class nw::snd::internal::BankFile::VelocityRegion
{
public:
    // the bits of the optional values (names are ours)
    enum ParameterBit
    {
        PARAMETER_ORIGINAL_KEY = 0,
        PARAMETER_VOLUME = 1,
        PARAMETER_PAN = 2,
        PARAMETER_PITCH = 3,
        PARAMETER_NOTE = 4, // byte 0 ignore note off, byte 1 key group, byte 2 interpolation type
        PARAMETER_ADSHR_CURVE = 9,
    };
    // all parameters set: the layout of RegionParameter
    static const u32 PARAMETER_FLAGS_ALL = 0x21F;

    // the parameters when all are set (names are ours)
    struct RegionParameter
    {
        u8 originalKey;            // 0x00
        u8 padding0[3];            // 0x01
        u8 volume;                 // 0x04
        u8 padding1[3];            // 0x05
        u8 pan;                    // 0x08
        u8 padding2[3];            // 0x09
        f32 pitch;                 // 0x0C
        bool isIgnoreNoteOff;      // 0x10
        u8 keyGroup;               // 0x11
        u8 interpolationType;      // 0x12
        u8 padding3;               // 0x13
        u32 adshrCurveOffset;      // 0x14
        BinaryReference adshrCurveReference; // 0x18
        AdshrCurve adshrCurve;     // 0x20
    };

    u8 GetKeyGroup() const; // 0x00742D88 | nintendogs:bytes [tier A]
    const AdshrCurve* GetAdshrCurve() const; // 0x00742DB4 | nintendogs:bytes [tier A]
    u8 GetOriginalKey() const; // 0x00742DF0 | nintendogs:bytes [tier A]
    bool IsIgnoreNoteOff() const; // 0x00742E18 | nintendogs:bytes [tier A]
    // the parameters if all are set (NULL: not)
    const RegionParameter* GetRegionParameter() const; // 0x00742E44 | nintendogs:bytes [tier A]
    u8 GetInterpolationType() const; // 0x00742E5C | nintendogs:bytes [tier A]
    u8 GetPan() const; // 0x00742E88 | nintendogs:bytes [tier A]
    f32 GetPitch() const; // 0x00742EB0 | nintendogs:bytes [tier A]
    u8 GetVolume() const; // 0x00742F24 | nintendogs:bytes [tier A]

    u32 waveIdTableIndex;       // 0x0
    Util::BitFlag optionParams; // 0x4
};
