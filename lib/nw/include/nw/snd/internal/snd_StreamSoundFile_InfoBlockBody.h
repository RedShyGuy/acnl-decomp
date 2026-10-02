#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

class nw::snd::internal::StreamSoundFile::InfoBlockBody
{
public:
    void GetTrackInfoTable() const; // 0x0073FBB8 | nintendogs:bytes [tier A]
    void GetStreamSoundInfo() const; // 0x0073FBD4 | nintendogs:bytes [tier A]
    void GetChannelInfoTable() const; // 0x0073FBEC | nintendogs:bytes [tier A]
};
