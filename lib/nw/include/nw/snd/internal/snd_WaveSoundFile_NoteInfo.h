#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_WaveSoundFile.h"

class nw::snd::internal::WaveSoundFile::NoteInfo
{
public:
    void GetAdshrCurve() const; // 0x0013D238 | nintendogs:callgraph [tier A]
    void GetOriginalKey() const; // 0x0013D274 | nintendogs:bytes [tier A]
    void GetSurroundPan() const; // 0x0013D29C | nintendogs:bytes [tier A]
    void GetPan() const; // 0x0013D2C8 | nintendogs:callgraph [tier A]
    void GetPitch() const; // 0x0013D2F0 | nintendogs:callgraph [tier A]
    void GetVolume() const; // 0x0013D364 | nintendogs:bytes [tier A]
};
