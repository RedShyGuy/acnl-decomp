#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_SoundArchiveFile.h"

class nw::snd::internal::SoundArchiveFile::SoundInfo
{
public:
    void GetSoundType() const; // 0x0013FE44 | nintendogs:bytes [tier A]
    void GetWaveSoundInfo() const; // 0x0013FE74 | nintendogs:callgraph [tier A]
    void GetPanMode() const; // 0x00141E68 | nintendogs:bytes [tier A]
    void GetPanCurve() const; // 0x00141E8C | nintendogs:bytes [tier A]
    void IsFrontBypass() const; // 0x00141EB8 | nintendogs:bytes [tier A]
    void GetActorPlayerId() const; // 0x00141EDC | nintendogs:bytes [tier A]
    void GetPlayerPriority() const; // 0x00141F08 | nintendogs:bytes [tier A]
    void ReadUserParam(int, unsigned int&) const; // 0x007403A0 | fefates:bytes [tier B]
    void GetSound3DInfo() const; // 0x007403C4 | nintendogs:bytes [tier A]
};
