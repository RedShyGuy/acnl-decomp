#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_BankFile.h"

class nw::snd::internal::BankFile::VelocityRegion
{
public:
    void GetKeyGroup() const; // 0x00742D88 | nintendogs:bytes [tier A]
    void GetAdshrCurve() const; // 0x00742DB4 | nintendogs:callseq-callee [tier A]
    void GetOriginalKey() const; // 0x00742DF0 | nintendogs:bytes [tier A]
    void IsIgnoreNoteOff() const; // 0x00742E18 | nintendogs:bytes [tier A]
    void GetRegionParameter() const; // 0x00742E44 | nintendogs:bytes [tier A]
    void GetInterpolationType() const; // 0x00742E5C | nintendogs:bytes [tier A]
    void GetPan() const; // 0x00742E88 | nintendogs:callseq-callee [tier A]
    void GetPitch() const; // 0x00742EB0 | nintendogs:callseq-callee [tier A]
    void GetVolume() const; // 0x00742F24 | nintendogs:bytes [tier A]
};
