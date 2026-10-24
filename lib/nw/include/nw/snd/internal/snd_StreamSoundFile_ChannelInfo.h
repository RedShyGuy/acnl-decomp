#pragma once

#include "decomp.h"
#include "nw/snd/internal/snd_StreamSoundFile.h"

class nw::snd::internal::StreamSoundFile::ChannelInfo
{
public:
    // the DSP ADPCM state (NULL: another encoding)
    const DspAdpcmChannelInfo* GetDspAdpcmChannelInfo() const; // 0x0073FBA0 | nintendogs:bytes [tier A]

    BinaryReference adpcmInfo; // 0x0
};
