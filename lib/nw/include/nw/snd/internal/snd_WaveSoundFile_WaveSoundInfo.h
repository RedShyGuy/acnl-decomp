#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

class nw::snd::internal::WaveSoundFile::WaveSoundInfo
{
public:
    void GetLpfFreq() const; // 0x0073F48C | fefates:bytes [tier B]
    void GetSendValue(unsigned char*, unsigned char*, unsigned char) const; // 0x0073F4B0 | nintendogs:bytes [tier A]
    void GetAdshrCurve() const; // 0x0073F59C | nintendogs:bytes [tier A]
    void GetBiquadType() const; // 0x0073F5D4 | fefates:bytes [tier B]
    void GetBiquadValue() const; // 0x0073F5FC | fefates:bytes [tier B]
    void GetSurroundPan() const; // 0x0073F624 | nintendogs:bytes [tier A]
    void GetPan() const; // 0x0073F64C | nintendogs:bytes [tier A]
    void GetPitch() const; // 0x0073F670 | nintendogs:bytes [tier A]
};
