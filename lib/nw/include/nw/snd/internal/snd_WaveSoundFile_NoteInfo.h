#pragma once

#include "decomp.h"
#include "nw/snd/internal/Util/snd_BitFlag.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

// a note of a wave sound: the wave and its optional parameters
class nw::snd::internal::WaveSoundFile::NoteInfo
{
public:
    // the bits of the optional values (names are ours)
    enum ParameterBit
    {
        PARAMETER_ORIGINAL_KEY = 0,
        PARAMETER_VOLUME = 1,
        PARAMETER_PAN = 2, // byte 0 pan, byte 1 surround pan
        PARAMETER_PITCH = 3,
        PARAMETER_ADSHR_CURVE = 9,
    };

    const AdshrCurve* GetAdshrCurve() const; // 0x0013D238 | nintendogs:callgraph [tier A]
    u8 GetOriginalKey() const; // 0x0013D274 | nintendogs:bytes [tier A]
    u8 GetSurroundPan() const; // 0x0013D29C | nintendogs:bytes [tier A]
    u8 GetPan() const; // 0x0013D2C8 | nintendogs:callgraph [tier A]
    f32 GetPitch() const; // 0x0013D2F0 | nintendogs:callgraph [tier A]
    u8 GetVolume() const; // 0x0013D364 | nintendogs:bytes [tier A]

    u32 waveIdTableIndex;       // 0x0
    Util::BitFlag optionParams; // 0x4
};
