#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

class nw::snd::internal::StreamSoundFile::ChannelInfo
{
public:
    void GetDspAdpcmChannelInfo() const; // 0x0073FBA0 | nintendogs:bytes [tier A]
};
